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
        morph::vec<float> corner = { -1.5, -0.5, -0.5 };
        float width = 3;
        float height = 1;
        float depth = 1.5;
        this->computeRectCuboid (corner, width, height, depth, morph::colour::blue);

        std::array<morph::vec<float>, 8> cubecorners;
        cubecorners[0] = {-0.1, 0.5, -0.1};
        cubecorners[1] = {-0.1, 0.5, 0.1};
        cubecorners[2] = {0.1, 0.5, 0.1};
        cubecorners[3] = {0.1, 0.5, -0.1};
        cubecorners[4] = {-0.2, 2, -0.2};
        cubecorners[5] = {-0.2, 2, 0.2};
        cubecorners[6] = {0.2, 2, 0.2};
        cubecorners[7] = {0.2, 2, -0.2};

        morph::TransformMatrix<float> tf;
        tf.translate (morph::vec<float>{-0.5f, 0.0f, 0.0f});

        for (int i = 4; i < 8; ++i) {
            cubecorners[i] = (tf * cubecorners[i]).less_one_dim();
        }


        std::array<morph::vec<float>, 8> cubecorners1;
        for (int i = 0; i < 8; ++i) {
            cubecorners1[i] = (tf * cubecorners[i]).less_one_dim();
        }

        std::array<morph::vec<float>, 8> cubecorners2;

        tf.setToIdentity();
        morph::Quaternion<float> rot;
        morph::vec<float> axis = { 0, 1, 0 };
        rot.rotate (axis, morph::mathconst<float>::pi);
        tf.rotate (rot);
        tf.translate (morph::vec<float>{0.5f, 0.0f, 0.0f});
        for (int i = 0; i < 8; ++i) {
            cubecorners2[i] = (tf * cubecorners[i]).less_one_dim();
        }

        this->computeCuboid (cubecorners1, morph::colour::crimson);
        this->computeCuboid (cubecorners2, morph::colour::crimson);

        morph::vec<float> a1s = { 1.5, 0, 0 };
        morph::vec<float> a1e = { 2.5, 1, 0 };
        this->computeTube (a1s, a1e, morph::colour::navy, morph::colour::blue, 0.1f);

        this->computeSphere (a1e, morph::colour::orchid1, 0.3, 16, 18);
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
