//
// Created by rolf on 21-07-20.
//

#include "ChipEstimator6DRotated.h"

std::optional<ChipFitResult>
ChipEstimator6DRotated::getChipEstimate(const std::vector<BallObservation> &observationsSinceKick,
                                        const CameraMap &camera, bool computeResiduals) {
    bool success = setMatrices(observationsSinceKick,camera);
    if(!success){
        return std::nullopt;
    }
    std::optional<Eigen::VectorXd> solveResult = solve();
    if(!solveResult){
        return std::nullopt;
    }
    return postProcess(*solveResult,computeResiduals);
}

bool ChipEstimator6DRotated::setMatrices(const std::vector<BallObservation> &observationsSinceKick,
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
        Eigen::Quaterniond camRotation = cam->worldToCamRotation();
        Eigen::Matrix3d camMatrix = camRotation.toRotationMatrix();

        double time = (observation.timeCaptured - firstTime).asSeconds();
        Eigen::Vector3d obsPos(observation.position.x(), observation.position.y(), 0.021333);
        Eigen::Vector3d obsPosCamFrame = camRotation * (obsPos - camPos);
        double alpha = obsPosCamFrame.x() / obsPosCamFrame.z();
        double beta = obsPosCamFrame.y() / obsPosCamFrame.z();

        double g = -9.81;
        double r00 = camMatrix(0, 0);
        double r01 = camMatrix(0, 1);
        double r02 = camMatrix(0, 2);
        double r10 = camMatrix(1, 0);
        double r11 = camMatrix(1, 1);
        double r12 = camMatrix(1, 2);
        double r20 = camMatrix(2, 0);
        double r21 = camMatrix(2, 1);
        double r22 = camMatrix(2, 2);

        //Big yikes but the equations are just ugly (but more accurate)
        AMatrix(i * 2, 0) = r00 - alpha * r20; //x
        AMatrix(i * 2, 1) = time * (r00 - alpha * r20);// vx
        AMatrix(i * 2, 2) = r01 - alpha * r21; //y
        AMatrix(i * 2, 3) = time * (r01 - alpha * r21); //vy
        AMatrix(i * 2, 4) = r02 - alpha * r22; //z
        AMatrix(i * 2, 5) = time * (r02 - alpha * r22); //vz
        BVector(i * 2) = r00 * camPos.x() + r01 * camPos.y() + r02 * camPos.z()
                         - alpha * (r20 * camPos.x() + r21 * camPos.y() + r22 * camPos.z())
                         + 0.5 * g * time * time * (alpha * r22 - r02);

        AMatrix(i * 2 + 1, 0) = r10 - beta * r20; //x
        AMatrix(i * 2 + 1, 1) = time * (r10 - beta * r20);// vx
        AMatrix(i * 2 + 1, 2) = r11 - beta* r21; //y
        AMatrix(i * 2 + 1, 3) = time * (r11 - beta * r21); //vy
        AMatrix(i * 2 + 1, 4) = r12 - beta * r22; //z
        AMatrix(i * 2 + 1, 5) = time * (r12 - beta * r22); //vz
        BVector(i * 2 + 1) = r10 * camPos.x() + r11 * camPos.y() + r12 * camPos.z()
                             - beta * (r20 * camPos.x() + r21 * camPos.y() + r22 * camPos.z())
                             + 0.5 * g * time * time * (beta * r22 - r12);

        ++i;
    }
    return true;
}

ChipFitResult ChipEstimator6DRotated::postProcess(const Eigen::VectorXd &solution, bool computeResiduals) {
    Eigen::Vector3d worldPos(solution(0),solution(2),solution(4));
    Eigen::Vector3d worldVel(solution(1),solution(3),solution(5));

    ChipFitResult result;
    result.kickPos = worldPos;
    result.kickVel = worldVel;

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
    if(AMatrix.rows()<AMatrix.cols()){
        return std::nullopt;
        //In other words if there's less than 3 observations the system is underdetermined and there are infinitely many solutions
    }
    std::cout << "Matrix rank: " << AMatrix.colPivHouseholderQr().rank()<< "cols: "<< AMatrix.cols() << std::endl; //For testing
    //Solve the linear system as given in least squares sense (e.g. find the best approximation)
    //Note that this gives a unique solution (it's a convex minimization problem) IF the system above is full rank.
    Eigen::VectorXd result = AMatrix.colPivHouseholderQr().solve(BVector);
    return result;
}

std::optional<Eigen::VectorXd> ChipEstimator6DRotated::solveIRLSL1norm() {
    if(AMatrix.rows() < AMatrix.cols()){
        return std::nullopt; //The system is underdetermined so there is no good reason to assume a solution exists
    }
    Eigen::DiagonalMatrix<double,Eigen::Dynamic> weights;
    weights.setIdentity(AMatrix.cols());
    int its = 0;
    //TODO: add stopping convergence condition
    Eigen::VectorXd solution;
    Eigen::VectorXd residues;
    while(its < 10){
        Eigen::VectorXd newSolution = (weights*AMatrix).colPivHouseholderQr().solve(weights*BVector);
        solution = newSolution;
        std::cout<<"_________"<<std::endl;
        std::cout<<solution<<std::endl;
        residues = BVector - AMatrix * solution;
        Eigen::VectorXd residueWeights = residues.cwiseInverse().cwiseAbs().cwiseSqrt();
        std::cout<<residueWeights<<std::endl;
        std::cout<<residues<<std::endl;
        weights.diagonal() = residueWeights;
        ++its;
    }
    //TODO: check if converged, if not return nullopt

    return solution;
}
