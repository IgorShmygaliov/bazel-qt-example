#pragma once
#include <QtWidgets>
#include <vector>
#include "polygon.h"

class Controller{
   public:
    const std::vector<Polygon>& GetPolygons();
    void AddPolygon(const Polygon& poly);
    void AddVertexToLastPolygon(const QPoint& new_vertex);

    void UpdateLastPolygon(const QPoint& new_vertex);
    void SetLight(const QPointF& ls);
    QPointF GetLight();

    int& Mode();
    std::vector<Ray> CastRays();

   private:
    int mode_ = 0; //0-light 1-poly
    std::vector<Polygon> polygons_;
    QPointF light_source_;
};