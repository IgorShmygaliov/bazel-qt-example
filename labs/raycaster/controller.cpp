#include "controller.h"

const std::vector<Polygon>& Controller::GetPolygons(){
    return polygons_;
}
void Controller::AddPolygon(const Polygon& poly){
    polygons_.push_back(poly);
}

void Controller::AddVertexToLastPolygon(const QPoint& new_vertex) {
    if (!polygons_.empty()){
        polygons_.back().AddVertex(new_vertex);
    }
}

void Controller::UpdateLastPolygon(const QPoint& new_vertex) {
    if (!polygons_.empty()){
        polygons_.back().UpdateLastVertex(new_vertex);
    }
}

void Controller::SetLight(const QPointF& ls){
    light_source_ = ls;
}
QPointF Controller::GetLight(){
    return light_source_;
}
int& Controller::Mode(){
    return mode_;
}

std::vector<Ray> Controller::CastRays(){
    std::vector<Ray> ans;
    for (auto poly : polygons_){
        std::vector<QPointF> vtx = poly.Get();
        for (auto v : vtx){
            double ang = std::atan2(v.y()-light_source_.y(),v.x()-light_source_.x());
            Ray r(light_source_, v, ang);
            ans.push_back(r);
            ans.push_back(r.Rotate(-0.0001));
            ans.push_back(r.Rotate(0.0001));
        }
    }
    return ans;
}