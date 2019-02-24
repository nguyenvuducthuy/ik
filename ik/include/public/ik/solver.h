#ifndef IK_SOLVER_H
#define IK_SOLVER_H

#include "ik/config.h"
#include "ik/vector.h"
#include "ik/solver_head.h"

/*!
 * @brief Only the algorithms listed here are actually enabled.
 *
 * This list is used at multiple locations in the library, specifically in
 * ik_solver_base_create() to fill in the switch/case with information on how
 * to create each solver, but there are also certain expectations on how
 * functions are to be named for each solver.
 *
 * @note Read WHAT_ARE_VTABLES.md in the ik root directory to learn more about
 * the mechanism with which the various solver functions are integrated into
 * the library.
 */
#define IK_SOLVER_ALGORITHM_LIST \
    X(ONE_BONE)
    /*
    X(TWO_BONE) \
    X(FABRIK) \
    X(MSS)*/

#define IK_SOLVER_FEATURES_LIST \
    X(CONSTRAINTS,      0x01) \
    X(TARGET_ROTATIONS, 0x02) \
    X(JOINT_ROTATIONS,  0x04) \
    X(ALL,              0xFF)

C_BEGIN

struct ik_node_t;

typedef void(*ik_solver_callback_func)(void* user_data,
                                       const ikreal_t position[3],
                                       const ikreal_t rotation[4]);

enum ik_solver_algorithm_e
{
#define X(solver) IK_SOLVER_##solver,
    IK_SOLVER_ALGORITHM_LIST
#undef X

    IK_SOLVER_ALGORITHMS_COUNT
};

enum ik_solver_features_e
{
#define X(arg, value) IK_SOLVER_##arg = value,
    IK_SOLVER_FEATURES_LIST
#undef X

    IK_SOLVER_FEATURES_COUNT
};

struct ik_solver_t
{
    IK_SOLVER_HEAD
};

#if defined(IK_BUILDING)

/*!
 * @brief Allocates a new solver object according to the specified algorithm.
 *
 * Once the solver is created, you can configure the solver to enable/disable
 * various features depending on your needs.
 *
 * The following attributes can be changed at any point.
 *  + solver->max_iterations
 *       Specifies the maximum number of iterations. The more iterations, the
 *       more exact the result will be. The default value for the FABRIK solver
 *       is 20, but you can get away with values as low as 5.
 *  + solver->tolerance
 *       This value can be changed at any point. Specifies the acceptable
 *       distance each effector needs to be to its target position. The solver
 *       will stop iterating if the effectors are within this distance. The
 *       default value is 1e-3. Recommended values are 100th of your world
 *       unit.
 *  + solver->flags
 *       Changes the behaviour of the solver. See the enum solver_flags_e for
 *       more information.
 *
 * The following attributes can be accessed (read from) but should not be
 * modified.
 *  + solver->tree
 *       The tree to be solved. You may modify the nodes in the tree.
 *       @note If you add/remove nodes or if you add/remove effectors, you
 *       must call ik_solver_rebuild_data() so the internal solver structures
 *       are updated. Failing to do so may cause segfaults. If you're just
 *       updating positions/rotations or any of the other public data then
 *       there is no need to rebuild data.
 *  + solver->effector_nodes_list
 *       A vector containing pointers to nodes in the tree which have an
 *       effector attached to them. You may not modify this list, but you may
 *       iterate it.
 * @param[in] algorithm The algorithm to use. Currently, only FABRIK is
 * supported.
 */
IK_PRIVATE_API ikret_t
ik_solver_create(struct ik_solver_t** solver, enum ik_solver_algorithm_e algorithm);

/*!
 * @brief Destroys the solver and all nodes/effectors that are part of the
 * solver. Any pointers to tree nodes are invalid after this function returns.
 */
IK_PRIVATE_API void
ik_solver_destroy(struct ik_solver_t* solver);

IK_PRIVATE_API ikret_t
ik_solver_construct(struct ik_solver_t* solver);

IK_PRIVATE_API void
ik_solver_destruct(struct ik_solver_t* solver);

/*!
 * @brief Causes the set tree to be processed into more optimal data structures
 * for solving. Must be called before ik_solver_solve().
 * @note Needs to be called whenever the tree changes in any way. I.e. if you
 * remove nodes or add nodes, or if you remove effectors or add effectors,
 * you must call this again before invoking the solver.
 * @return Returns non-zero if any of the chain trees are invalid for any
 * reason. If this happens, check the log for error messages.
 * @warning If this functions fails, the internal structures are in an
 * undefined state. You cannot solve the tree in this state.
 */
IK_PRIVATE_API ikret_t
ik_solver_prepare(struct ik_solver_t* solver, struct ik_node_t* tree);

/*!
 * @brief Computes the distances between the nodes and stores them in
 * node->dist_to_parent. The positions used for this computation are those of
 * the active pose (node->position). For this reason, make sure that you've
 * correctly initialized the active pose before calling this function.
 *
 * The segment lengths are typically computed once during initialisation and
 * then never again. Of course, there are exceptions, such as when your tree
 * has translational motions. In this case, you will have to recalculate the
 * segment lengths every time node positions change.
 *
 * @note This function gets called by ik_solver_prepare().
 */
IK_PRIVATE_API void
ik_solver_update_translations(struct ik_solver_t* solver);

/*!
 * @brief Solves the IK problem. The node solutions will be provided via a
 * callback function, see ik_solver_iterate_solution().
 * @return Returns the number of effectors that reached their target. If no
 * targets were reached, 0 is returned (doesn't mean the solver failed, just
 * that all targets are out of reach).
 */
IK_PRIVATE_API uint32_t
ik_solver_solve(struct ik_solver_t* solver);

IK_PRIVATE_API void
ik_solver_iterate_nodes(const struct ik_solver_t* solver,
                           ik_solver_callback_func callback);

IK_PRIVATE_API uint16_t
ik_solver_get_max_iterations(const struct ik_solver_t* solver);
IK_PRIVATE_API void
ik_solver_set_max_iterations(struct ik_solver_t* solver, uint16_t max_iterations);
IK_PRIVATE_API ikreal_t
ik_solver_get_tolerance(const struct ik_solver_t* solver);
IK_PRIVATE_API void
ik_solver_set_tolerance(struct ik_solver_t* solver, ikreal_t tolerance);

IK_PRIVATE_API uint16_t
ik_solver_get_features(const struct ik_solver_t* solver);
IK_PRIVATE_API void
ik_solver_enable_features(struct ik_solver_t* solver, uint16_t features);
IK_PRIVATE_API void
ik_solver_disable_features(struct ik_solver_t* solver, uint16_t features);
IK_PRIVATE_API uint8_t
ik_solver_is_feature_enabled(const struct ik_solver_t* solver, enum ik_solver_features_e feature);


#endif /* IK_BUILDING */

#define SOLVER_FOR_EACH_EFFECTOR_NODE(solver_var, effector_var) \
    VECTOR_FOR_EACH(&(solver_var)->effector_nodes_list, struct ik_node_t*, solver_var##effector_var) \
    struct ik_node_t* effector_var = *solver_var##effector_var; {

#define SOLVER_FOR_EACH_CHAIN(solver_var, base_chain_var) \
    VECTOR_FOR_EACH(&(solver_var)->chain_list, struct chain_t, base_chain_var) {

#define SOLVER_END_EACH VECTOR_END_EACH }

C_END

#endif /* IK_SOLVER_H */
