//
// Created by rolf on 19-07-20.
//

#include "ChipEstimator6DSingle.h"

bool
ChipEstimator6DSingle::setMatrices(const std::vector<BallObservation> &observationsSinceKick, const CameraMap &camera){
    int N = observationsSinceKick.size();
    //We can write it as a linear system of equations Ax=B, where A is a 2N x 6 matrix and B is a 2N x 1 vector,
    // and x is a vector containing [x_0, v_x, y_0, v_y, z_0, v_z], which are the chip position and velocity respectively
    AMatrix.resize(2 * N, 6);
    BVector.resize(2 * N);

    Time firstTime = observationsSinceKick[0].timeCaptured;
    std::optional<Camera> cam = camera[observationsSinceKick[0].cameraID];
    if(!cam){
        return false; //We don't have camera data so we cannot fit a chip
    }
    usedCamera = *cam;


    //We need the following to compute the matrix A
    Eigen::Vector3d camPos = usedCamera.worldPos();
    Eigen::Quaterniond camRotation = usedCamera.worldToCamRotation();
    Eigen::Vector3d camGravity = camRotation * Eigen::Vector3d(0, 0, -9.81);
    double gx = camGravity.x();
    double gy = camGravity.y();
    double gz = camGravity.z();

    int i = 0;
    for (const auto &observation : observationsSinceKick) {
        double time = (observation.timeCaptured - firstTime).asSeconds();
        Eigen::Vector3d obsPos(observation.position.x(),observation.position.y(), 0.021333);
        Eigen::Vector3d obsPosCamFrame = camRotation * (obsPos - camPos);
        double alpha = obsPosCamFrame.x() / obsPosCamFrame.z();
        double beta = obsPosCamFrame.y() / obsPosCamFrame.z();


        AMatrix(i * 2, 0) = -1; //x
        AMatrix(i * 2, 1) = -time;// vx
        AMatrix(i * 2, 2) = 0; //y
        AMatrix(i * 2, 3) = 0; //vy
        AMatrix(i * 2, 4) = alpha; //z
        AMatrix(i * 2, 5) = alpha * time; //vz
        BVector(i * 2) = 0.5 * time * time * (gx - alpha * gz);


        AMatrix(i * 2 + 1, 0) = 0;
        AMatrix(i * 2 + 1, 1) = 0;
        AMatrix(i * 2 + 1, 2) = -1;
        AMatrix(i * 2 + 1, 3) = -time;
        AMatrix(i * 2 + 1, 4) = beta;
        AMatrix(i * 2 + 1, 5) = beta * time;
        BVector(i * 2 + 1) = 0.5 * time * time * (gy - beta * gz);
        i++;
    }
    return true;
}

std::optional<ChipFitResult>
ChipEstimator6DSingle::getChipEstimate(const std::vector<BallObservation> &observationsSinceKick,
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

std::optional<Eigen::VectorXd> ChipEstimator6DSingle::solve(){
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

ChipFitResult ChipEstimator6DSingle::postProcess(const Eigen::VectorXd &solution, bool computeResiduals) {
    //We get the fit velocity and position in the frame of the camera and translate it back to world coordinates
    Eigen::Vector3d pos(solution(0), solution(2), solution(4));
    Eigen::Vector3d vel(solution(1), solution(3), solution(5));

    Eigen::Quaterniond camRotation = usedCamera.worldToCamRotation().inverse();

    Eigen::Vector3d worldPos =  camRotation * pos + usedCamera.worldPos();
    Eigen::Vector3d worldVel =  camRotation * vel;

    ChipFitResult result;
    result.kickPos = worldPos;
    result.kickVel = worldVel;

    //optionally, compute the residuals (errors) in the fit and add them to the result.
    if (computeResiduals) {
        result.residuals = BVector - AMatrix * solution;
    }
    return result;
}