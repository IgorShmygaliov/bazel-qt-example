#include "board.h"
#include "ray.h"
#include <iostream>
#include <numbers>

board::board(QWidget* parent) : QWidget(parent) {
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    ctrl_.Mode() = 0;
    
}

void board::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QRect rect = this->rect();
    painter.fillRect(rect, Qt::black);
    if (ctrl_.GetPolygons().empty()){
        ctrl_.AddPolygon(Polygon({rect.topLeft(), rect.topRight(),
            rect.bottomRight(), rect.bottomLeft()}));
        ctrl_.AddPolygon(Polygon({}));
    }

    // QPointF p1 = {300,300};
    // QPointF p2 = {200,300};
    // QPointF p22 = {200,200};
    // QPointF p3 = {300,200};
    // QPointF p33 = {400,200};
    // QPointF p4 = {400,300};
    // QPointF p44 = {400,400};
    
    // painter.setPen(QPen(Qt::red, 2)); 
    // painter.drawLine(p1,p2);
    // painter.drawLine(p22,p2);
    // painter.drawLine(p1,p3);
    // painter.drawLine(p33,p3);
    // painter.drawLine(p44,{300, 400});
    // painter.drawLine(p33,p4);

    // Ray rrr = {{300,300},{300,400},std::numbers::pi/2};
    // painter.drawLine(rrr.GetBegin(), rrr.GetEnd());
    // rrr = rrr.Rotate(-acos(-1)/2);
    // painter.drawLine(rrr.GetBegin(), rrr.GetEnd());
    


    QPoint ppp = mapFromGlobal(QCursor::pos());
    

    // многоугольники
    painter.setPen(QPen(Qt::red, 2)); 
    std::vector<Polygon> vp = ctrl_.GetPolygons();
    for (int j=0; j<vp.size();j++){
        std::vector<QPointF> vts = vp[j].Get();
        for (int i=0; (i+1)<vts.size(); i++){
            painter.drawLine(vts[i],vts[i+1]);
        }
        if (vts.size()>=1 && j != vp.size()-1){
            painter.drawLine(vts[vts.size()-1],vts[0]);
        }   
        if (vts.size()>=1 && j == vp.size()-1 && this->rect().contains(ppp)){
            painter.drawLine(vts[vts.size()-1], ppp);
        }
    }

    // лучи
    // if (ctrl_.Mode() == 0){
    //     painter.setPen(QPen(QColor("#FF80A0"), 2)); 
    //     std::vector<Ray> vr = ctrl_.CastRays();
    //     ctrl_.IntersectRays(&vr);
    //     ctrl_.RemoveAdjacentRays(&vr);
    //     for (const auto & i : vr){
    //         painter.drawLine(i.GetBegin(),i.GetEnd());
    //     }

    // }

    int c=8;

    QPointF mainlight = ctrl_.GetLight();
    std::vector<QPointF> lights;
    lights.push_back(mainlight);

    lights.push_back({mainlight.x()+c,mainlight.y()+c});
    lights.push_back({mainlight.x()+c,mainlight.y()-c});
    lights.push_back({mainlight.x()-c,mainlight.y()+c});
    lights.push_back({mainlight.x()-c,mainlight.y()-c});

    lights.push_back({mainlight.x()+2*c,mainlight.y()});
    lights.push_back({mainlight.x()-2*c,mainlight.y()});
    lights.push_back({mainlight.x(),mainlight.y()+2*c});
    lights.push_back({mainlight.x(),mainlight.y()-2*c});

    // свет
    if (ctrl_.Mode() == 0){
        painter.setBrush(QColor("#2F8092B4"));
        painter.setPen(Qt::NoPen);
        for (auto lt : lights){
            ctrl_.SetLight(lt);
            if (lt.x() > 1 && lt.y() > 1 && lt.x() + 1 < width() && lt.y() + 1 < height()){
                Polygon p = ctrl_.CreateLightArea();
                const std::vector<QPointF> vv = p.Get();
                painter.drawPolygon(vv.data(), vv.size(), Qt::OddEvenFill);
            }
            
        }
        ctrl_.SetLight(mainlight);
        
    }

    // источник света
    if (ctrl_.Mode() == 0){
        painter.setBrush(Qt::red);
        painter.setPen(Qt::NoPen);
        int r = 5;
        for (auto lt : lights){
            if (lt.x() > 1 && lt.y() > 1 && lt.x() + 1 < width() && lt.y() + 1 < height()){
                painter.drawEllipse(lt, r, r);
            }
        }
    }
    
    
}

void board::mouseMoveEvent(QMouseEvent* event) {
    if (ctrl_.Mode() == 0) {
        ctrl_.SetLight(event->pos());
    }
    if (ctrl_.Mode() == 1 || (ctrl_.GetLight().x() > 1 && ctrl_.GetLight().y() > 1 && ctrl_.GetLight().x() + 1 < width() && ctrl_.GetLight().y() + 1 < height())){
        update();
    }
}

void board::mousePressEvent(QMouseEvent* event) {
    if (ctrl_.Mode() == 1) {
        if (event->button() == Qt::LeftButton) {
            ctrl_.AddVertexToLastPolygon(event->pos());
        }else if (event->button() == Qt::RightButton) {
            std::vector<Polygon> vp = ctrl_.GetPolygons();
            if (vp[vp.size()-1].Get().size()>0){
                ctrl_.AddPolygon(Polygon({}));
            }
            
        }
    }
    update();
}