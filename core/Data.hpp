#ifndef DATA_ROOT_HPP_INCLUDED
#define DATA_ROOT_HPP_INCLUDED

#include <cstdint>
#include <memory>

class Task;
class DataProxy;

class Data {

public:

    enum class d_type : uint32_t {
        v_u8     = 1 << 0,
        v_u16    = 1 << 1,
        v_f      = 1 << 2,
        v_mk     = 1 << 3,
        v_lbld   = 1 << 4,
        vertices = 1 << 5,
        polyline = 1 << 6,
        polygon  = 1 << 7,
        mesh     = 1 << 8
    };

    constexpr static uint32_t vc_mask = static_cast<uint32_t>(d_type::v_u8)  | 
                                        static_cast<uint32_t>(d_type::v_u16) | 
                                        static_cast<uint32_t>(d_type::v_f)   | 
                                        static_cast<uint32_t>(d_type::v_mk)  | 
                                        static_cast<uint32_t>(d_type::v_lbld);

    static inline bool is_voxels_canvas(d_type dt) { return (static_cast<uint32_t>(dt) & vc_mask) != 0; }

public:

    virtual int run(Task* v, Bucket b) = 0;
    void set_proxy(std::unique_ptr<DataProxy> p);
    virtual d_type dtype() const = 0;

protected:

    std::unique_ptr<DataProxy> proxy;

protected:

    virtual ~Data() = default;

    Data() = delete;
    Data(DataProxy* p): proxy(p) {}
};

template <typename T>
class VoxelsCanvas;

class VoxelsCanvasU8;
class VoxelsCanvasU16;
class VoxelsCanvasFloat;

class MaskCanvas;
class LabeledCanvas;

class Vertices;
class PolyLine;
class Mesh;

#endif //DATA_ROOT_HPP_INCLUDED