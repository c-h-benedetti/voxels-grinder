#include "Bucket.hpp"


Bucket::Bucket(const Bucket& b) : canvasColumns(b.canvasColumns), canvasRows(b.canvasRows), canvasSlices(b.canvasSlices), canvasFrames(b.canvasFrames),
    localColumns(b.localColumns), localRows(b.localRows), localSlices(b.localSlices), localFrames(b.localFrames), origin(b.origin),
    calibration(b.calibration), overlap_x(b.overlap_x), overlap_y(b.overlap_y), overlap_z(b.overlap_z) {
}

Bucket::Bucket(
    const Bucket& b, 
    std::pair<size_t, size_t> lc, 
    std::pair<size_t, size_t> lr, 
    std::pair<size_t, size_t> ls, 
    std::pair<size_t, size_t> lf
) : canvasColumns(b.canvasColumns), canvasRows(b.canvasRows), canvasSlices(b.canvasSlices), canvasFrames(b.canvasFrames),
    localColumns(b.localColumns), localRows(b.localRows), localSlices(b.localSlices), localFrames(b.localFrames), origin(b.origin),
    calibration(b.calibration), overlap_x(b.overlap_x), overlap_y(b.overlap_y), overlap_z(b.overlap_z) {
    
    if (lc.first != lc.second) { this->set_local_columns(lc); }
    if (lr.first != lr.second) { this->set_local_rows(lr); }
    if (ls.first != ls.second) { this->set_local_slices(ls); }
    if (lf.first != lf.second) { this->set_local_frames(lf); }
}

Bucket::Iterator::Iterator(const Bucket& b): rows(b.localRows), columns(b.localColumns), slices(b.localSlices), frames(b.localFrames) {
    x = columns.first;
    y = rows.first;
    z = slices.first;
    f = frames.first;
    sizeX = b.get_canvas_columns();
    sizeY = b.get_canvas_rows();
    sizeZ = b.get_canvas_slices();
    sizeT = b.get_canvas_frames();
}
