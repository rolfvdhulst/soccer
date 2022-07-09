//
// Created by rolf on 21-07-20.
//

#include "ball/kick/estimation/chip/ChipEstimator6DRotated.h"

std::optional<ChipFitResult>
ChipEstimator6DRotated::getChipEstimate(bool computeResiduals) {
  std::optional<Eigen::VectorXd> solveResult = solve();
  if (!solveResult.has_value()) {
    return std::nullopt;
  }
  return postProcess(solveResult.value(), computeResiduals);
}

ChipFitResult ChipEstimator6DRotated::postProcess(const Eigen::VectorXd &solution, bool computeResiduals) {
  Eigen::Vector3d worldPos(solution(0), solution(2), solution(4));
  Eigen::Vector3d worldVel(solution(1), solution(3), solution(5));

  ChipFitResult result(worldPos,worldVel,firstTime);
  //optionally, compute the residuals (errors) in the fit and add them to the result.
  if (computeResiduals) {
    result.residuals = BVector - AMatrix * solution;
  }
  return result;
}

std::optional<Eigen::VectorXd> ChipEstimator6DRotated::solve() {
  return solveL2norm();
}

std::optional<Eigen::VectorXd> ChipEstimator6DRotated::solveL2norm() {
  if (AMatrix.rows() < AMatrix.cols()) {
    return std::nullopt;
    //In other words if there's less than 3 observations the system is underdetermined and there are infinitely many solutions
  }
  std::cout << "Matrix rank: " << AMatrix.colPivHouseholderQr().rank() << "cols: " << AMatrix.cols()
            << std::endl; //For testing
  //Solve the linear system as given in least squares sense (e.g. find the best approximation)
  //Note that this gives a unique solution (it's a convex minimization problem) IF the system above is full rank.
  Eigen::VectorXd result = AMatrix.colPivHouseholderQr().solve(BVector);
  return result;
}

std::optional<Eigen::VectorXd> ChipEstimator6DRotated::solveIRLSL1norm() {
  if (AMatrix.rows() < AMatrix.cols()) {
    return std::nullopt; //The system is underdetermined so there is no good reason to assume a solution exists
  }
  Eigen::DiagonalMatrix<double, Eigen::Dynamic> weights;
  weights.setIdentity(AMatrix.cols());
  int its = 0;
  //TODO: add stopping convergence condition
  Eigen::VectorXd solution;
  Eigen::VectorXd residues;
  while (its < 10) {
    Eigen::VectorXd newSolution = (weights * AMatrix).colPivHouseholderQr().solve(weights * BVector);
    solution = newSolution;
    std::cout << "_________" << std::endl;
    std::cout << solution << std::endl;
    residues = BVector - AMatrix * solution;
    Eigen::VectorXd residueWeights = residues.cwiseInverse().cwiseAbs().cwiseSqrt();
    std::cout << residueWeights << std::endl;
    std::cout << residues << std::endl;
    weights.diagonal() = residueWeights;
    ++its;
  }
  //TODO: check if converged, if not return nullopt

  return solution;
}
void ChipEstimator6DRotated::addObservation(const BallObservation &observation, const Camera &camera) {
  int N = AMatrix.rows();
  AMatrix.conservativeResize(N + 2, Eigen::NoChange);
  BVector.conservativeResize(N + 2);

  //We need the following to compute the matrix A
  const Eigen::Vector3d camPos = camera.worldPos();
  const Eigen::Quaterniond camRotation = camera.worldToCamRotation();
  const Eigen::Matrix3d camMatrix = camRotation.toRotationMatrix();

  const double time = (observation.timeCaptured - firstTime).asSeconds();
  const Eigen::Vector3d
      obsPos(observation.position.x(), observation.position.y(), 0.021333); //TODO: record ball radius constant
  const Eigen::Vector3d obsPosCamFrame = camRotation * (obsPos - camPos);
  const double alpha = obsPosCamFrame.x() / obsPosCamFrame.z();
  const double beta = obsPosCamFrame.y() / obsPosCamFrame.z();
  const double g = -9.81; //TODO: record gravity constant

  double r00 = camMatrix(0, 0);
  double r01 = camMatrix(0, 1);
  double r02 = camMatrix(0, 2);
  double r10 = camMatrix(1, 0);
  double r11 = camMatrix(1, 1);
  double r12 = camMatrix(1, 2);
  double r20 = camMatrix(2, 0);
  double r21 = camMatrix(2, 1);
  double r22 = camMatrix(2, 2);

  //Big yikes but the equations are just ugly (but more accurate for multiple camera's)
  //Maybe there's a way to easily do this with vector/matrix math which isn't as ugly, but this is relatively easy to check by hand
  AMatrix(N, 0) = r00 - alpha * r20; //x
  AMatrix(N, 1) = time * (r00 - alpha * r20);// vx
  AMatrix(N, 2) = r01 - alpha * r21; //y
  AMatrix(N, 3) = time * (r01 - alpha * r21); //vy
  AMatrix(N, 4) = r02 - alpha * r22; //z
  AMatrix(N, 5) = time * (r02 - alpha * r22); //vz
  BVector(N) = r00 * camPos.x() + r01 * camPos.y() + r02 * camPos.z()
      - alpha * (r20 * camPos.x() + r21 * camPos.y() + r22 * camPos.z())
      + 0.5 * g * time * time * (alpha * r22 - r02);

  AMatrix(N + 1, 0) = r10 - beta * r20; //x
  AMatrix(N + 1, 1) = time * (r10 - beta * r20);// vx
  AMatrix(N + 1, 2) = r11 - beta * r21; //y
  AMatrix(N + 1, 3) = time * (r11 - beta * r21); //vy
  AMatrix(N + 1, 4) = r12 - beta * r22; //z
  AMatrix(N + 1, 5) = time * (r12 - beta * r22); //vz
  BVector(N + 1) = r10 * camPos.x() + r11 * camPos.y() + r12 * camPos.z()
      - beta * (r20 * camPos.x() + r21 * camPos.y() + r22 * camPos.z())
      + 0.5 * g * time * time * (beta * r22 - r12);

}
bool ChipEstimator6DRotated::setObservations(const std::vector<BallObservation> &observationsSinceKick,
                                             const CameraMap &cameras) {
  AMatrix.resize(0, 6);
  BVector.resize(0, 1);
  if (!observationsSinceKick.empty()) {
    //take lowest observation time
    firstTime = std::min_element(observationsSinceKick.begin(), observationsSinceKick.end(),
                                 [](const BallObservation &a, const BallObservation &b) {
                                   return a.timeCaptured < b.timeCaptured;
                                 })->timeCaptured;
    for (const auto &observation : observationsSinceKick) {
      auto camera = cameras.getCamera(observation.cameraID);
      if (camera.has_value()) {
        addObservation(observation, camera.value());
      } else {
        return false;
      }
    }
  }

  return true;
}
ChipEstimator6DRotated::ChipEstimator6DRotated(const BallObservation &observation, const Camera &camera) :
    firstTime(observation.timeCaptured) {
  AMatrix.resize(0, 6);
  BVector.resize(0, 1);
  addObservation(observation, camera);
}
unsigned long ChipEstimator6DRotated::nrOfObservations() const {
  return AMatrix.rows()/2;
}