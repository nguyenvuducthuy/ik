#ifndef IK_LIB_H
#define IK_LIB_H

#include "ik/config.h"

#include "ik/constraint.h"
#include "ik/effector.h"
#include "ik/info.h"
#include "ik/init.h"
#include "ik/log.h"
#include "ik/mat3x3.h"
#include "ik/node.h"
#include "ik/pole.h"
#include "ik/solver.h"
#include "ik/quat.h"
#include "ik/tests.h"
#include "ik/transform.h"
#include "ik/vec3.h"

C_BEGIN

struct ik_constraint_api_t
{
    struct ik_constraint_t* (*create)    (void);
    void                    (*destroy)   (struct ik_constraint_t*);
    struct ik_constraint_t* (*duplicate) (const struct ik_constraint_t*);
    ikret_t                 (*set_type)  (struct ik_constraint_t*, enum ik_constraint_type_e);
    void                    (*set_custom)(struct ik_constraint_t*, void (*)(const struct ik_node_t*, ikreal_t[4]));
    ikret_t                 (*attach)    (struct ik_constraint_t*, struct ik_node_t*);
    void                    (*detach)    (struct ik_constraint_t*);

#define X(arg) enum ik_constraint_type_e arg;
    IK_CONSTRAINTS_LIST
#undef X
};

struct ik_effector_api_t
{
    struct ik_effector_t* (*create)   (void);
    void                  (*destroy)  (struct ik_effector_t* effector);
    struct ik_effector_t* (*duplicate)(const struct ik_effector_t*);
    ikret_t               (*attach)   (struct ik_effector_t*, struct ik_node_t*);
    void                  (*detach)   (struct ik_effector_t*);

#define X(arg, value) enum ik_effector_flags_e arg;
    IK_EFFECTOR_FLAGS_LIST
#undef X
};

struct ik_info_api_t
{
    const char* (*author)      (void);
    const char* (*version)     (void);
    int         (*build_number)(void);
    const char* (*host)        (void);
    const char* (*date)        (void);
    const char* (*commit)      (void);
    const char* (*compiler)    (void);
    const char* (*cmake)       (void);
    const char* (*all)         (void);
};

struct ik_log_api_t
{
    ikret_t (*init)      (void);
    void    (*deinit)    (void);
    void    (*severity)  (enum ik_log_severity_e);
    void    (*timestamps)(int);
    void    (*prefix)    (const char*);
    void    (*debug)     (const char*, ...);
    void    (*info)      (const char*, ...);
    void    (*warning)   (const char*, ...);
    void    (*error)     (const char*, ...);
    void    (*fatal)     (const char*, ...);

#define X(arg) enum ik_log_severity_e arg;
    IK_LOG_SEVERITY_LIST
#undef X
};

struct ik_mat3x3_api_t
{
    void (*from_quat)(ikreal_t[9], const ikreal_t[4]);
};

struct ik_node_api_t
{
    struct ik_node_t* (*create)      (uint32_t);
    ikret_t           (*construct)   (struct ik_node_t*, uint32_t);
    void              (*destruct)    (struct ik_node_t*);
    void              (*destroy)     (struct ik_node_t*);
    struct ik_node_t* (*create_child)(struct ik_node_t*, uint32_t);
    ikret_t           (*add_child)   (struct ik_node_t*, struct ik_node_t*);
    void              (*unlink)      (struct ik_node_t*);
    vector_size_t     (*child_count) (const struct ik_node_t*);
    struct ik_node_t* (*find_child)  (const struct ik_node_t*, uint32_t);
    struct ik_node_t* (*duplicate)   (const struct ik_node_t*, int);

};

struct ik_pole_api_t
{
    struct ik_pole_t* (*create)   (void);
    void              (*destroy)  (struct ik_pole_t*);
    void              (*set_type) (struct ik_pole_t*, enum ik_pole_type_e);
    struct ik_pole_t* (*duplicate)(const struct ik_pole_t*);
    ikret_t           (*attach)   (struct ik_pole_t*, struct ik_node_t*);
    void              (*detach)   (struct ik_pole_t*);

#define X(arg) enum ik_pole_type_e arg;
    IK_POLE_TYPE_LIST
#undef X
};

struct ik_solver_api_t
{
    struct ik_solver_t* (*create)                (enum ik_solver_algorithm_e);
    void                (*destroy)               (struct ik_solver_t*);
    ikret_t             (*construct)             (struct ik_solver_t*);
    void                (*destruct)              (struct ik_solver_t*);
    ikret_t             (*rebuild)               (struct ik_solver_t*);
    void                (*update_distances)      (struct ik_solver_t*);
    ikret_t             (*solve)                 (struct ik_solver_t*);
    void                (*set_tree)              (struct ik_solver_t*, struct ik_node_t*);
    struct ik_node_t*   (*unlink_tree)           (struct ik_solver_t*);
    void                (*iterate_all_nodes)     (struct ik_solver_t*, void (*)(struct ik_node_t*));
    void                (*iterate_affected_nodes)(struct ik_solver_t*, void (*)(struct ik_node_t*));
    void                (*iterate_base_nodes)    (struct ik_solver_t*, void (*)(struct ik_node_t*));

#define X(arg) enum ik_solver_algorithm_e arg;
    IK_SOLVER_ALGORITHM_LIST
#undef X
#define X(arg, value) enum ik_solver_flags_e arg;
    IK_SOLVER_FLAGS_LIST
#undef X
};

struct ik_quat_api_t
{
    void     (*copy)                (ikreal_t[4], const ikreal_t[4]);
    void     (*set)                 (ikreal_t[4], ikreal_t, ikreal_t, ikreal_t, ikreal_t);
    void     (*set_identity)        (ikreal_t[4]);
    void     (*set_axis_angle)      (ikreal_t[4], ikreal_t, ikreal_t, ikreal_t, ikreal_t);
    void     (*add_quat)            (ikreal_t[4], const ikreal_t[4]);
    ikreal_t (*mag)                 (const ikreal_t[4]);
    void     (*conj)                (ikreal_t[4]);
    void     (*negate)              (ikreal_t[4]);
    void     (*invert)              (ikreal_t[4]);
    void     (*normalize)           (ikreal_t[4]);
    void     (*mul_quat)            (ikreal_t[4], const ikreal_t[4]);
    void     (*nmul_quat)           (ikreal_t[4], const ikreal_t[4]);
    void     (*mul_no_normalize)    (ikreal_t[4], const ikreal_t[4]);
    void     (*nmul_no_normalize)   (ikreal_t[4], const ikreal_t[4]);
    void     (*mul_scalar)          (ikreal_t[4], ikreal_t);
    void     (*div_scalar)          (ikreal_t[4], ikreal_t);
    ikreal_t (*dot)                 (const ikreal_t[4], const ikreal_t[4]);
    void     (*ensure_positive_sign)(ikreal_t[4]);
    void     (*angle)               (ikreal_t[4], const ikreal_t[3], const ikreal_t[3]);
    void     (*angle_no_normalize)  (ikreal_t[4], const ikreal_t[3], const ikreal_t[3]);
    int      (*print)               (char*, const ikreal_t[4]);
};

struct ik_tests_api_t
{
    ikret_t (*run)(int* argc, char** argv);
};

struct ik_transform_api_t
{
    void (*node)(struct ik_node_t* root, uint8_t mode);

#define X(arg, value) enum ik_transform_mode_e arg;
    IK_TRANSFORM_MODE_LIST
#undef X
};

struct ik_vec3_api_t
{
    void     (*copy)                        (ikreal_t v[3], const ikreal_t src[3]);
    void     (*set)                         (ikreal_t v[3], ikreal_t x, ikreal_t y, ikreal_t z);
    void     (*set_zero)                    (ikreal_t v[3]);
    void     (*add_scalar)                  (ikreal_t v1[3], ikreal_t scalar);
    void     (*add_vec3)                    (ikreal_t v1[3], const ikreal_t v2[3]);
    void     (*sub_scalar)                  (ikreal_t v1[3], ikreal_t scalar);
    void     (*sub_vec3)                    (ikreal_t v1[3], const ikreal_t v2[3]);
    void     (*mul_scalar)                  (ikreal_t v1[3], ikreal_t scalar);
    void     (*mul_vec3)                    (ikreal_t v1[3], const ikreal_t v2[3]);
    void     (*div_scalar)                  (ikreal_t v[3], ikreal_t scalar);
    void     (*div_vec3)                    (ikreal_t v[3], const ikreal_t v2[3]);
    ikreal_t (*length_squared)              (const ikreal_t v[3]);
    ikreal_t (*length)                      (const ikreal_t v[3]);
    void     (*normalize)                   (ikreal_t v[3]);
    ikreal_t (*dot)                         (const ikreal_t v1[3], const ikreal_t v2[3]);
    void     (*cross)                       (ikreal_t v1[3], const ikreal_t v2[3]);
    void     (*ncross)                      (ikreal_t v1[3], const ikreal_t v2[3]);
    void     (*rotate)                      (ikreal_t v[3], const ikreal_t q[4]);
    void     (*nrotate)                     (ikreal_t v[3], const ikreal_t q[4]);
    void     (*project_from_vec3)           (ikreal_t v1[3], const ikreal_t v2[3]);
    void     (*project_from_vec3_normalized)(ikreal_t v1[3], const ikreal_t v2[3]);
    void     (*project_onto_plane)          (ikreal_t v[3], const ikreal_t x[3], const ikreal_t y[3]);
};

struct ik_api_t
{
    ikret_t (*init)(void);
    uintptr_t (*deinit)(void);

    struct ik_constraint_api_t constraint;
    struct ik_effector_api_t   effector;
    struct ik_info_api_t       info;
    struct ik_log_api_t        log;
    struct ik_mat3x3_api_t     mat3x3;
    struct ik_node_api_t       node;
    struct ik_pole_api_t       pole;
    struct ik_solver_api_t     solver;
    struct ik_quat_api_t       quat;
    struct ik_tests_api_t      tests;
    struct ik_transform_api_t  transform;
    struct ik_vec3_api_t       vec3;
};

IK_PUBLIC_API extern struct ik_api_t IKAPI;

C_END

#endif /* IK_LIB_H */
