//
// Created by rolf on 19-07-20.
//

#include "ball/kick/estimation/chip/ChipEstimator5D.h"


bool ChipEstimator5D::setMatrices(const std::vector<BallObservation> &observationsSinceKick,
                                  const CameraMap &camera,
                                  double tOffSet) {
    int N = observationsSinceKick.size();
    //We can write it as a linear system of equations Ax=B, where A is a 2N x 5 matrix and B is a 2N x 1 vector,
    // and x is a vector containing [x_0, v_x, y_0, v_y, z_0, v_z], which are the chip position and velocity respectively
    AMatrix.resize(2 * N, 5);
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

        double time = (observation.timeCaptured - firstTime).asSeconds() + tOffSet;
        //The obsPos z value here is a bit vague because it's not exactly 'clear' what height the ball is at as seen from the camera at all times
        //Any value between 0 and 0.04266666 should be fine-ish
        Eigen::Vector3d obsPos(observation.position.x(), observation.position.y(), 0.021333);
        Eigen::Vector3d relPos = camPos - obsPos;
        double g = -9.81;

        AMatrix(i * 2, 0) = relPos.z(); //x
        AMatrix(i * 2, 1) = time * relPos.z();// vx
        AMatrix(i * 2, 2) = 0; //y
        AMatrix(i * 2, 3) = 0; //vy
        AMatrix(i * 2, 4) = -time * relPos.x(); //vz
        BVector(i * 2) = 0.5 * g * time * time * relPos.x() + obsPos.x() * relPos.z();

        AMatrix(i * 2 + 1, 0) = 0; //x
        AMatrix(i * 2 + 1, 1) = 0;// vx
        AMatrix(i * 2 + 1, 2) = relPos.z(); //y
        AMatrix(i * 2 + 1, 3) = time * relPos.z(); //vy
        AMatrix(i * 2 + 1, 4) = -time * relPos.y(); //vz
        BVector(i * 2 + 1) = 0.5 * g * time * time * relPos.y() + obsPos.y() * relPos.z();

        ++i;
    }
    return true;
}

ChipFitResult ChipEstimator5D::postProcess(const ChipFitResultInternal &result) {
    Eigen::Vector3d worldPos(result.solution(0), result.solution(2), 0.021333);
    Eigen::Vector3d worldVel(result.solution(1), result.solution(3), result.solution(4));

    ChipFitResult worldResult;
    worldResult.kickPos = worldPos;
    worldResult.kickVel = worldVel;
    worldResult.residuals = result.residuals;
    return worldResult;
}

std::optional<ChipFitResult>
ChipEstimator5D::timeLineSearch(const std::vector<BallObservation> &observationsSinceKick, const CameraMap &camera) {
    if (observationsSinceKick.size() < 3) {
        return std::nullopt;
    }
    double tOffSet = 0.05;
    double inc = tOffSet * 0.5; //Binary search on tOffSet
    //Technically a nonlinear problem but it's linear in all coordnates but tOffSet.
    //It's also not unreasonable in general to assume that the error in tOffSet only has 1 minimum (atleast for the L2 problem)
    // Thus we do a simple line search over tOffSet within reasonable intervals for camera data:
    while (inc > 1e-3) {
        //Solve it twice to find 'gradient' e.g. is the error increasing or decreasing?
        //TODO: perhaps we can find an analytical derivative/gradient w.r.t tOffSet? would be a lot cheaper
        std::optional<ChipFitResultInternal> resultLower = computeL2Sol(observationsSinceKick, camera, tOffSet - 1e-5);
        std::optional<ChipFitResultInternal> resultHigher = computeL2Sol(observationsSinceKick, camera, tOffSet + 1e-5);
        if(!resultHigher || !resultLower){
            return std::nullopt;
        }
        if (resultLower->l1Error() < resultHigher->l1Error()) {
            tOffSet -= inc;
        } else {
            tOffSet += inc;
        }
        inc *= 0.5;
    }
    std::optional<ChipFitResultInternal> result = computeL2Sol(observationsSinceKick,camera,tOffSet);
    return postProcess(*result);
}

std::optional<ChipEstimator5D::ChipFitResultInternal>
ChipEstimator5D::computeL2Sol(const std::vector<BallObservation> &observationsSinceKick, const CameraMap &camera,
                              double tOffSet) {
    bool success = setMatrices(observationsSinceKick,camera,tOffSet);
    if(!success){
        return std::nullopt;
    }
    Eigen::VectorXd solution = AMatrix.colPivHouseholderQr().solve(BVector);
    ChipFitResultInternal result;
    result.solution = solution;
    result.residuals = BVector - AMatrix*solution;
    return result;
}

double ChipEstimator5D::ChipFitResultInternal::l1Error() const {
    return residuals.lpNorm<1>();
}

double ChipEstimator5D::ChipFitResultInternal::l2Error() const {
    return residuals.norm();
}
