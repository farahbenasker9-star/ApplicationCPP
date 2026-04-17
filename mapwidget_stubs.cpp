void MapWidget::paintEvent(QPaintEvent *event) { Q_UNUSED(event); }
void MapWidget::mousePressEvent(QMouseEvent *event) { Q_UNUSED(event); }
void MapWidget::mouseMoveEvent(QMouseEvent *event) { Q_UNUSED(event); }
void MapWidget::mouseReleaseEvent(QMouseEvent *event) { Q_UNUSED(event); }
void MapWidget::wheelEvent(QWheelEvent *event) { Q_UNUSED(event); }
QPointF MapWidget::gpsToScreen(double lat, double lng, const QRect &mapArea) { Q_UNUSED(lat); Q_UNUSED(lng); Q_UNUSED(mapArea); return QPointF(); }
void MapWidget::drawMap(QPainter &p, const QRect &mapArea) { Q_UNUSED(p); Q_UNUSED(mapArea); }
void MapWidget::drawMarkers(QPainter &p, const QRect &mapArea) { Q_UNUSED(p); Q_UNUSED(mapArea); }
