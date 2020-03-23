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
        void process(const proto::SSL_GeometryData &geometryData);
        [[nodiscard]] bool receivedMessage() const;
        [[nodiscard]] Time lastUpdateTime() const;
        [[nodiscard]] const FieldState& getField() const;
        [[nodiscard]] const CameraMap& getCameras() const;
    private:
        FieldState field = FieldState(DefaultField::DivisionA);
        std::string lastGeometryString;
        int geomUpdates = 0;
        Time lastGeomTime = Time(-1000000000000);
        CameraMap cameraMap;

};

#endif //SOCCER_GEOMETRYFILTER_H
