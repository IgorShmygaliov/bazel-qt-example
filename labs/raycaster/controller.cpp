#include "controller.h"
#include "labs/raycaster/ray.h"
#include <algorithm>
#include <cmath>
#include <iostream>

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
            Ray rr = r.Rotate(-0.0001);
            if (polygons_[0].IntersectRay(rr).has_value()){
                rr.SetEnd(polygons_[0].IntersectRay(rr).value());
            }
            ans.push_back(rr);
            Ray rrr = r.Rotate(0.0001);
            if (polygons_[0].IntersectRay(rrr) != std::nullopt){
                rrr.SetEnd(polygons_[0].IntersectRay(rrr).value());
            }
            ans.push_back(rrr);
        }
    }
    std::sort(ans.begin(),ans.end(),[](Ray r1, Ray r2){return r1.GetAngle()<r2.GetAngle();});
    return ans;
}

void Controller::IntersectRays(std::vector<Ray>* rays){
    for (auto& r : *rays){
        for (auto& p : polygons_){
            std::optional<QPointF> nend = p.IntersectRay(r);
            if (nend.has_value()){
                QPointF ee = nend.value();
                if ( sqrt((ee.x()-r.GetBegin().x())*(ee.x()-r.GetBegin().x()) + (ee.y()-r.GetBegin().y())*(ee.y()-r.GetBegin().y())) < r.Length() ){
                    r.SetEnd(ee);
                }                
            }

        }
    }
}

void Controller::RemoveAdjacentRays(std::vector<Ray>* rays){
    std::vector<Ray> rr = *rays;
    std::vector<Ray> ans; ans.clear();
    for (int i=0;i<rays->size();i++){
        if (ans.empty() || Ray(rr[i].GetEnd(),ans.back().GetEnd(),0).Length()>kEps){
            ans.push_back(rr[i]);
        }
    }
    *rays = ans;
}

Polygon Controller::CreateLightArea(){
    std::vector<Ray> vr = CastRays();
    IntersectRays(&vr);
    RemoveAdjacentRays(&vr);
    Polygon poly({});
    for (auto r : vr){
        poly.AddVertex(r.GetEnd());
    }
    return poly;
}