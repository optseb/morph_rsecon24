#include <morph/Visual.h>
#include <morph/VisualModel.h>

template <int glver = morph::gl::version_4_1>
struct my_vm : public morph::VisualModel<glver>
{
    // Make a constructor that sets the location in the scene via a parent method
    my_vm(const morph::vec<float> _offset)
        : morph::VisualModel<glver>::VisualModel (_offset) {}

    void initializeVertices()
    {
        // A blue box
        morph::vec<float> corner = { -1.5, -1.5, -0.5 };
        float width = 3;
        float height = 2;
        float depth = 1;
        this->computeRectCuboid (corner, width, height, depth, morph::colour::blue);

        // A lever
        morph::vec<float> s = { 1.5, 0, 0 };
        morph::vec<float> e = { 2.5, 1, 0 };
        morph::vec<float> facevec1 = {0, 0, 1};
        morph::vec<float> facevec2 = {0, 1, 0};
        this->computeTube (s, e, facevec1, facevec2, morph::colour::navy, morph::colour::blue, 0.1f);
        this->computeSphere (e, morph::colour::orchid1, 0.3, 16, 18);

        // Eyes
        s = { 0.7, 0, 0.5 };
        e = { 0.7, 0, 0.6 };
        this->computeTube (s, e, morph::colour::goldenrod, morph::colour::goldenrod3, 0.3f, 32);
        s[0] *= -1; e[0] *= -1;
        this->computeTube (s, e, morph::colour::goldenrod, morph::colour::goldenrod3, 0.3f, 32);

        // A triangular nose (oriented randomly)
        s = { 0, -0.3, 0.5 };
        e = { 0, -0.3, 0.8 };
        this->computeTube (s, e, morph::colour::green4, morph::colour::green2, 0.16f, 3);

        // Antennae
        std::array<morph::vec<float>, 8> cuboid_corners;
        cuboid_corners[0] = {-0.1, 0.5, -0.1};
        cuboid_corners[1] = {-0.1, 0.5, 0.1};
        cuboid_corners[2] = {0.1, 0.5, 0.1};
        cuboid_corners[3] = {0.1, 0.5, -0.1};
        cuboid_corners[4] = {-0.2, 2, -0.2};
        cuboid_corners[5] = {-0.2, 2, 0.2};
        cuboid_corners[6] = {0.2, 2, 0.2};
        cuboid_corners[7] = {0.2, 2, -0.2};

        // Translate one face of the cube (4 of the vertices)
        morph::TransformMatrix<float> tf;
        tf.translate (morph::vec<float>{-0.5f, 0.0f, 0.0f});
        for (int i = 4; i < 8; ++i) {
            cuboid_corners[i] = (tf * cuboid_corners[i]).less_one_dim();
        }

        // Apply translation again to the cuboid to make the first antenna
        std::array<morph::vec<float>, 8> antenna1;
        for (int i = 0; i < 8; ++i) {
            antenna1[i] = (tf * cuboid_corners[i]).less_one_dim();
        }

        std::array<morph::vec<float>, 8> antenna2;
        tf.setToIdentity(); // re-use TransformMatrix
        const morph::vec<float> y_axis = { 0, 1, 0 };
        // Rotate and translate cuboid_corners for the second antenna
        tf.rotate (y_axis, morph::mathconst<float>::pi);
        tf.translate (morph::vec<float>{ 0.5f, 0.0f, 0.0f });
        for (int i = 0; i < 8; ++i) {
            antenna2[i] = (tf * cuboid_corners[i]).less_one_dim();
        }

        // Antennae
        this->computeCuboid (antenna1, morph::colour::crimson);
        this->computeCuboid (antenna2, morph::colour::crimson);
   }
};

int main()
{
    morph::Visual v(1024, 768, "Program 2");
    v.lightingEffects();
    v.diffuse_position = { 15.0f, 7.0f, 10.0f };

    auto mvm = std::make_unique<my_vm<>>(morph::vec<float>{0,0,0});
    v.bindmodel (mvm);
    mvm->finalize();
    v.addVisualModel (mvm);

    v.keepOpen();

    return 0;
}
