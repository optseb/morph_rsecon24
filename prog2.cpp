#include <morph/Visual.h>
#include <morph/Grid.h>       // We'll use a Grid
#include <morph/GridVisual.h> // We'll visualize the Grid
#include <morph/Scale.h>      // We'll need to scale values


int main()
{
    morph::Visual v(1536, 1536, "Using VisualModels");

    unsigned int pixels = 2000;
    float width = morph::mathconst<float>::two_pi;
    float dx = width / (pixels - 1);
    float os = -0.5f * width;
    // Constructor: width, height, grid spacing, offset of corner.
    morph::Grid the_plane (pixels, pixels,
                           morph::vec<float, 2>{ dx, dx },
                           morph::vec<float, 2>{ os, os  });
    std::cout << "Grid width: " << the_plane.width() << " cf. width: " << width << std::endl;

    // Make a container of scalar values to plot across the plane
    morph::vvec<float> z_data (the_plane.n, 0.0f);

    // Loop through grid, setting data from the function z = sin(cos(tan(x))) * sin(cos(tan(y)))
    for (unsigned int i = 0; i < the_plane.n; ++i) {
        morph::vec<float, 2> xy = the_plane[i];
        float z = sin(cos(tan( xy[0] ))) * sin(cos(tan( xy[1] )));
        z_data[i] = z;
    }

    // Plot z as a function of xy
    auto gridvis = std::make_unique<morph::GridVisual<float>>(&the_plane, morph::vec<float>{0,0,0});
    v.bindmodel (gridvis);

    gridvis->setScalarData (&z_data);

    gridvis->gridVisMode = morph::GridVisMode::Columns; // Try Triangles
    gridvis->interpolate_colour_sides = true; // default is false. Try it (with Columns, above).
    gridvis->cm.setType (morph::ColourMapType::Inferno); // Try Jet or HSV1D

    gridvis->finalize();

    v.addVisualModel (gridvis);

    v.keepOpen();
    return 0;
}
