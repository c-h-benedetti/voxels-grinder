#ifndef BUCKET_LOCATION_HPP_INCLUDED
#define BUCKET_LOCATION_HPP_INCLUDED

#include <utility>
#include <cstdlib>
#include <stdexcept>
#include "Calibration.hpp"


class Bucket {

    friend class Iterator;
    size_t canvasColumns, canvasRows, canvasSlices, canvasFrames; // Dimensions of the canvas we are refering to.
    std::pair<size_t, size_t> localColumns, localRows, localSlices, localFrames;
    std::pair<size_t, size_t> overlap_x, overlap_y, overlap_z;
    glm::vec3 origin; // Origin of the bucket if it is used as a bounding-box.
    size_t start_frame=0;
    Calibration calibration; // Calibration used.

private:

    inline void set_span(std::pair<size_t, size_t>& target, const std::pair<size_t, size_t>& input, const size_t& bound) {
        if (input.first >= input.second) { throw std::invalid_argument("Attempt to set a dimension to 0 or a negative value."); }
        if (input.first >= bound) { throw std::invalid_argument("A local range can't start after end-of-axis."); }
        if (input.second > bound) { throw std::invalid_argument("A local range can't stop after end-of-axis."); }
        target = input;
    }

public:

    class Iterator {

        friend class Bucket;
        const std::pair<size_t, size_t> columns, rows, slices, frames;
        size_t sizeX, sizeY, sizeZ, sizeT;
        size_t x, y, z, f;
        bool valid = true;

        Iterator(const Bucket& b);

    public:

        inline operator bool() const   { return this->valid; }
        inline operator size_t() const { return this->index_of(); }

        inline glm::ivec4 get_position() const { return glm::ivec4(x, y, z, f); }

        inline size_t index_of() const { 
            return sizeX*sizeY*sizeZ*f + sizeX*sizeY*z + sizeX*y+x;
        }

        inline void next() {
            if (++x < columns.second) { return; }
            x = columns.first;

            if (++y < rows.second) { return; }
            y = rows.first;

            if (++z < slices.second) { return; }
            z = slices.first;

            if (++f < frames.second) { return; }
            valid = false;
        }
    };

    inline Iterator get_iterator() const { return Iterator(*this); }

public:

    Bucket(size_t c=1, size_t r=1, size_t s=1, size_t f=1):
        canvasColumns(c), 
        canvasRows(r), 
        canvasSlices(s), 
        canvasFrames(f),
        localColumns({0, c}), 
        localRows({0, r}), 
        localSlices({0, s}), 
        localFrames({0, f}) {
            if (!(canvasColumns && canvasRows && canvasSlices && canvasFrames)) {
                throw std::invalid_argument("Attempt to set a dimension to 0 or a negative value.");
            }
    }

    Bucket(size_t c, 
           size_t r, 
           size_t s, 
           size_t f, 
           std::pair<size_t, size_t> lc, 
           std::pair<size_t, size_t> lr, 
           std::pair<size_t, size_t> ls, 
           std::pair<size_t, size_t> lf
        ) : Bucket(c, r, s, f) {
        this->set_local_columns(lc);
        this->set_local_rows(lr);
        this->set_local_slices(ls);
        this->set_local_frames(lf);
    }

    Bucket(const Bucket& b);

    Bucket(
        const Bucket& b, 
        std::pair<size_t, size_t> lc, 
        std::pair<size_t, size_t> lr, 
        std::pair<size_t, size_t> ls, 
        std::pair<size_t, size_t> lf
    );

    inline size_t index_of(size_t x, size_t y, size_t z, size_t f) const {
        return canvasColumns*canvasRows*canvasSlices*f + canvasColumns*canvasRows*z + canvasColumns*y+x;
    }

    inline void set_origin(glm::vec3 o) { this->origin = o; }
    inline void set_start_frame(size_t f) { this->start_frame = f; }

    inline glm::vec3 get_origin() const { return this->origin; }
    inline size_t get_start_frame() const { return this->start_frame; }

    inline void set_calibration(Calibration c) { this->calibration = c; }
    inline Calibration get_calibration() const { return this->calibration; }

    inline void set_local_columns(const std::pair<size_t, size_t>& c) { this->set_span(localColumns, c, canvasColumns); }
    inline void set_local_rows(const std::pair<size_t, size_t>& r)    { this->set_span(localRows, r, canvasRows); }
    inline void set_local_slices(const std::pair<size_t, size_t>& s)  { this->set_span(localSlices, s, canvasSlices); }
    inline void set_local_frames(const std::pair<size_t, size_t>& f)  { this->set_span(localFrames, f, canvasFrames); }

    inline size_t get_canvas_size() const { return canvasColumns * canvasRows * canvasSlices * canvasFrames; }
    inline size_t get_local_size() const  { 
        return (localColumns.second - localColumns.first) * 
               (localRows.second - localRows.first) * 
               (localSlices.second - localSlices.first) * 
               (localFrames.second - localFrames.first); 
    }

    inline size_t get_canvas_columns() const { return canvasColumns; }
    inline size_t get_canvas_rows() const    { return canvasRows; }
    inline size_t get_canvas_slices() const  { return canvasSlices; }
    inline size_t get_canvas_frames() const  { return canvasFrames; }

    inline size_t get_local_columns() const { return (localColumns.second - localColumns.first); }
    inline size_t get_local_rows() const    { return (localRows.second - localRows.first); }
    inline size_t get_local_slices() const  { return (localSlices.second - localSlices.first); }
    inline size_t get_local_frames() const  { return (localFrames.second - localFrames.first); }

    inline std::pair<size_t, size_t> get_columns_range() const { return localColumns; }
    inline std::pair<size_t, size_t> get_rows_range() const    { return localRows; }
    inline std::pair<size_t, size_t> get_slices_range() const  { return localSlices; }
    inline std::pair<size_t, size_t> get_frames_range() const  { return localFrames; }

    inline float get_canvas_width() const    { return (float)this->canvasColumns * this->calibration.get_size_x(); }
    inline float get_canvas_height() const   { return (float)this->canvasRows * this->calibration.get_size_y(); }
    inline float get_canvas_depth() const    { return (float)this->canvasSlices * this->calibration.get_size_z(); }
    inline float get_canvas_duration() const { return (float)this->canvasFrames * this->calibration.get_time_interval(); }

    inline float get_local_width() const    { return (float)(localColumns.second - localColumns.first) * this->calibration.get_size_x(); }
    inline float get_local_height() const   { return (float)(localRows.second - localRows.first) * this->calibration.get_size_y(); }
    inline float get_local_depth() const    { return (float)(localSlices.second - localSlices.first) * this->calibration.get_size_z(); }
    inline float get_local_duration() const { return (float)(localFrames.second - localFrames.first) * this->calibration.get_time_interval(); }
};


#endif //BUCKET_LOCATION_HPP_INCLUDED
