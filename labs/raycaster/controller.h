#pragma once
#include <QtWidgets>
#include <vector>
#include "polygon.h"

class Controller{
   public:
    std::vector<Polygon>& GetPolygons();
    void AddPolygon(const Polygon& poly);
    void AddVertexToLastPolygon(const QPointF& new_vertex);

    void UpdateLastPolygon(const QPointF& new_vertex);
    void SetLight(const QPointF& ls);
    QPointF GetLight();

    int& Mode();
    std::vector<Ray> CastRays();
    void IntersectRays(std::vector<Ray>* rays);
    void RemoveAdjacentRays(std::vector<Ray>* rays);
    Polygon CreateLightArea();

   private:
    int mode_ = 0; //0-light 1-poly
    std::vector<Polygon> polygons_;
    QPointF light_source_;
};