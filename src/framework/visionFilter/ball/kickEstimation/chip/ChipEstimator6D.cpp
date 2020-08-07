//
// Created by rolf on 19-07-20.
//

#include "ChipEstimator6D.h"

std::optional<ChipFitResult>
ChipEstimator6D::getChipEstimate(const std::vector<BallObservation> &observationsSinceKick,
                                 const CameraMap &camera, bool computeResiduals) {
    bool success = setMatrices(observationsSinceKick, camera);
    if (!success) {
        return std::nullopt;
    }
    std::optional<Eigen::VectorXd> solveResult = solve();
    if (!solveResult) {
        return std::nullopt;
    }
    return postProcess(*solveResult, computeResiduals);
}

bool ChipEstimator6D::setMatrices(const std::vector<BallObservation> &observationsSinceKick,
                                  const CameraMap &camera) {
    int N = observationsSinceKick.size();
    //We can write it as a linear system of equations Ax=B, where A is a 2N x 6 matrix and B is a 2N x 1 vector,
    // and x is a vector containing [x_0, v_x, y_0, v_y, z_0, v_z], which are the chip position and velocity respectively
    AMatrix.resize(2 * N, 6);
    BVector.resize(2 * N);

    Time firstTime = observationsSinceKick[0].timeCaptured;


    int i = 0;
    for (const auto &observation : observationsSinceKick) {
        std::optional<Camera> cam = camera[observation.cameraID];
        if (!cam) {
            return false; //We don't have camera data so we cannot fit a chip
        }
        //We need the following to compute the matrix A
        Eigen::Vector3d camPos = cam->worldPos();

        double time = (observation.timeCaptured - firstTime).asSeconds();
        //The obsPos z value here is a bit vague because it's not exactly 'clear' what height the ball is at as seen from the camera at all times
        //Any value between 0 and 0.04266666 should be fine-ish
        Eigen::Vector3d obsPos(observation.position.x(), observation.position.y(), 0.021333);
        Eigen::Vector3d relPos = camPos - obsPos;
        double g = -9.81;

        AMatrix(i * 2, 0) = relPos.z(); //x
        AMatrix(i * 2, 1) = time * relPos.z();// vx
        AMatrix(i * 2, 2) = 0; //y
        AMatrix(i * 2, 3) = 0; //vy
        AMatrix(i * 2, 4) = -relPos.x(); //z
        AMatrix(i * 2, 5) = -time * relPos.x(); //vz
        BVector(i * 2) = 0.5 * g * time * time * relPos.x() + obsPos.x() * camPos.z() - obsPos.z() * camPos.x();

        AMatrix(i * 2 + 1, 0) = 0; //x
        AMatrix(i * 2 + 1, 1) = 0;// vx
        AMatrix(i * 2 + 1, 2) = relPos.z(); //y
        AMatrix(i * 2 + 1, 3) = time * relPos.z(); //vy
        AMatrix(i * 2 + 1, 4) = -relPos.y(); //z
        AMatrix(i * 2 + 1, 5) = -time * relPos.y(); //vz
        BVector(i * 2 + 1) = 0.5 * g * time * time * relPos.y() + obsPos.y() * camPos.z() - obsPos.z() * camPos.y();

        ++i;
    }
    return true;
}

ChipFitResult ChipEstimator6D::postProcess(const Eigen::VectorXd &solution, bool computeResiduals) {
    Eigen::Vector3d worldPos(solution(0), solution(2), solution(4));
    Eigen::Vector3d worldVel(solution(1), solution(3), solution(5));

    ChipFitResult result;
    result.kickPos = worldPos;
    result.kickVel = worldVel;

    //optionally, compute the residuals (errors) in the fit and add them to the result.
    if (computeResiduals) {
        result.residuals = BVector - AMatrix * solution;
    }
    return result;
}

std::optional<Eigen::VectorXd> ChipEstimator6D::solve() {
    return solveL2norm();
}

std::optional<Eigen::VectorXd> ChipEstimator6D::solveL2norm() {
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

std::optional<Eigen::VectorXd> ChipEstimator6D::solveIRLSL1norm() {
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