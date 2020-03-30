//
// Created by rolf on 22-03-20.
//

#ifndef SOCCER_GEOMETRYFILTER_H
#define SOCCER_GEOMETRYFILTER_H

#include <field/FieldState.h>
#include <field/CameraMap.h>
#include <core/Time.h>
#include <protobuf/messages_robocup_ssl_geometry.pb.h>


class GeometryFilter {
    public:
        bool process(const proto::SSL_GeometryData &geometryData);
        const proto::SSL_GeometryData& getGeometry();
        bool receivedFirstGeometry() const;
        [[nodiscard]] Time lastUpdateTime() const;
    private:
        std::string lastGeometryString;
        Time lastGeomTime = Time(-1000000000000);
        proto::SSL_GeometryData combinedGeometry;
        std::map<uint,proto::SSL_GeometryCameraCalibration> cameras;
};

#endif //SOCCER_GEOMETRYFILTER_H
