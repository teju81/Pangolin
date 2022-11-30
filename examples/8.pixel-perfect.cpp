#include <pangolin/gui/drawn_primitives.h>
#include <pangolin/gui/drawn_image.h>
#include <pangolin/context/context.h>

/*
  == Pangolin-by-example ==

*/

using namespace sophus;
using namespace pangolin;

 namespace pangolin{

// Custom type
struct Cross
{
    Eigen::Vector3d pos = {0.0, 0.0, 0.0};
    Eigen::Vector4d color = {1.0, 0.0, 0.0, 1.0};
    double radius = 1.0;
};

// Implement traits so that Pangolin knows how to render the type.
template<>
struct DrawableConversionTraits<Cross> {
static Shared<DrawLayer::Drawable> makeDrawable(const Cross& x) {
    auto prims = DrawnPrimitives::Create({
        .element_type=DrawnPrimitives::Type::lines,
        .default_color = x.color
    });
    prims->vertices->update(std::vector<Eigen::Vector3f>{
        x.pos.cast<float>() + Eigen::Vector3f{-0.5f, -0.5f, 0.0f},
        x.pos.cast<float>() + Eigen::Vector3f{+0.5f, +0.5f, 0.0f},
        x.pos.cast<float>() + Eigen::Vector3f{-0.5f, +0.5f, 0.0f},
        x.pos.cast<float>() + Eigen::Vector3f{+0.5f, -0.5f, 0.0f}
    }, {});
    return prims;
}};}

int main( int argc, char** argv )
{
    const int w = 8;
    const int h = 5;
    const int win_scale = 100;
    auto context = Context::Create({
        .title="Pixel-perfect overlay",
        .window_size = {win_scale*w, win_scale*h},
    } );

    // Draw a very low resolution image so that we can visually ensure we are
    // not missing any half pixels etc
    auto image = sophus::Image<float>::makeGenerative({w,h}, [](int x, int y){
        return float(y*w + x) / float(w*h);
    });

    auto layer = makeLayer(image);
    layer->addInPixels(
        Cross{.pos={0.0, 0.0, 0.0}},
        Cross{.pos={float(w-1), float(h-1), 0.0}}
    );
    context->setLayout( layer );

    // markers: points, circles, stars, triangles, ...
    //          position, size, color
    // x,y,z,r,  r,g,b,a,  type


    // kernels: guassians, ramps
    //          position, spread, density
    // arrows:  rays, line segments, infinite lines
    //          start, end, direction, width
    // objects: frame of reference, camera, waypoint, mesh, plane, disk, cylinder, ellipsoid
    //          position, orientation, size


    context->loop();
    return 0;
}
