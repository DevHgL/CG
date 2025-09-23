#include "acutest.h"
#include "vec.h"
#include "transforms.h"

static bool closef(float a, float b, float eps=1e-5f){ return fabs(a-b) < eps; }
static bool close_vec(const vec3& a, const vec3& b, float eps=1e-5f){ return norm(a-b) < eps; }
static bool close_mat(const mat4& A, const mat4& B, float eps=1e-5f){ return norm(A-B) < eps; }

void test_rotate_n(){
    vec3 axis = normalize(vec3{1,2,3});
    float theta = 0.7f;
    mat4 R = rotate(axis, theta);

    vec3 Ra = toMat3(R)*axis; // eixo é autovetor
    TEST_CHECK(close_vec(Ra, axis));

    vec3 v = normalize(cross(axis, vec3{1,0,0}));
    if(norm(v) < 1e-6f) v = normalize(cross(axis, vec3{0,1,0}));
    vec3 Rv = toMat3(R)*v;
    TEST_CHECK(closef(norm(v), norm(Rv)));
    float cosang = dot(v, Rv);
    TEST_CHECK(closef(cosang, cos(theta), 1e-4f));
}

void test_rotate_x(){
    float t = 0.31f; mat4 A = rotate_x(t);
    mat4 B = rotate(vec3{1,0,0}, t);
    TEST_CHECK(close_mat(A, B));
}

void test_rotate_y(){
    float t = 0.62f; mat4 A = rotate_y(t);
    mat4 B = rotate(vec3{0,1,0}, t);
    TEST_CHECK(close_mat(A, B));
}

void test_rotate_z(){
    float t = 1.24f; mat4 A = rotate_z(t);
    mat4 B = rotate(vec3{0,0,1}, t);
    TEST_CHECK(close_mat(A, B));
}

TEST_LIST = {
    {"rotate n", test_rotate_n},
    {"rotate x", test_rotate_x},
    {"rotate y", test_rotate_y},
    {"rotate z", test_rotate_z},
    {NULL, NULL}
};


