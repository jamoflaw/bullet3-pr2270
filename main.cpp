#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionDispatch/btCollisionDispatcherMt.h"
#include "BulletDynamics/Dynamics/btSimulationIslandManagerMt.h"
#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorldMt.h"
#include "BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolverMt.h"

// Physics simulation
btDefaultCollisionConfiguration* collision_configuration;
btCollisionDispatcher* dispatcher;
btBroadphaseInterface* overlapping_pair_cache;
btConstraintSolverPoolMt* solver_pool;
btSequentialImpulseConstraintSolverMt* solver;
btDiscreteDynamicsWorld* dynamics_world;
btITaskScheduler* task_scheduler;

#define SAFE_DELETE(ptr) if(ptr){delete ptr;ptr=nullptr;}


// Contact Added callback
bool PhysicsContactAddedCallback(btManifoldPoint& cp,	const btCollisionObjectWrapper* colObj0, int partId0, int index0, const btCollisionObjectWrapper* colObj1, int partId1, int index1)
{
	printf("PhysicsContactAddedCallback\n");
    return true;
}

// Contact Removed callback
bool PhysicsContactDestroyedCallback(void* userPersistentData)
{
	printf("PhysicsContactDestroyedCallback\n");
    return true;
}

void PhysicsTickCallback(btDynamicsWorld *world, btScalar timeStep)
{
    printf("PhysicsTickCallback\n");
}

int main(int argc, char** argv)
{
    // Initialise the Physics world
    collision_configuration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcherMt(collision_configuration, 40);
    overlapping_pair_cache = new btDbvtBroadphase();
    solver_pool = new btConstraintSolverPoolMt(BT_MAX_THREAD_COUNT);
    solver = new btSequentialImpulseConstraintSolverMt();
    dynamics_world = new btDiscreteDynamicsWorldMt(dispatcher, overlapping_pair_cache, solver_pool, solver, collision_configuration);
    task_scheduler = btCreateDefaultTaskScheduler();
    btSetTaskScheduler(task_scheduler);

    dynamics_world->setGravity(btVector3(0.0f, 9.8f, 0.0f));

	    // Set the physics tick callback for collision detection
    dynamics_world->setInternalTickCallback(PhysicsTickCallback);
    
    // Set the collision callback
    gContactAddedCallback = PhysicsContactAddedCallback;
    gContactDestroyedCallback = PhysicsContactDestroyedCallback;


	btAlignedObjectArray<btCollisionShape*> collision_shapes;

	{
		btCollisionShape* ground_shape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));

		collision_shapes.push_back(ground_shape);

		btTransform ground_transform;
		ground_transform.setIdentity();
		ground_transform.setOrigin(btVector3(0, -56, 0));

		btScalar mass(0.);

		bool is_dynamic = (mass != 0.f);

		btVector3 local_inertia(0, 0, 0);
		if (is_dynamic)
			ground_shape->calculateLocalInertia(mass, local_inertia);

		btDefaultMotionState* my_motion_state = new btDefaultMotionState(ground_transform);
		btRigidBody::btRigidBodyConstructionInfo rb_info(mass, my_motion_state, ground_shape, local_inertia);
		btRigidBody* body = new btRigidBody(rb_info);

		//add the body to the dynamics world
		dynamics_world->addRigidBody(body);
	}

#define RANDER (float)(-600 + (rand()%1200))
	for(int j=0; j<150; ++j)
	{
		//create a dynamic rigidbody

		//btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
		btCollisionShape* col_shape = new btSphereShape(btScalar(1.));
		collision_shapes.push_back(col_shape);

		/// Create Dynamic Objects
		btTransform start_transform;
		start_transform.setIdentity();

		btScalar mass(1.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool is_dynamic = (mass != 0.f);

		btVector3 local_inertia(0, 0, 0);
		if (is_dynamic)
			col_shape->calculateLocalInertia(mass, local_inertia);

		start_transform.setOrigin(btVector3(RANDER, 10, RANDER));

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* my_motion_state = new btDefaultMotionState(start_transform);
		btRigidBody::btRigidBodyConstructionInfo rb_info(mass, my_motion_state, col_shape, local_inertia);
		btRigidBody* body = new btRigidBody(rb_info);

		dynamics_world->addRigidBody(body);
	}

	/// Do some simulation

	///-----stepsimulation_start-----
	for (int i = 0; i < 150; i++)
	{
		dynamics_world->stepSimulation(1.f / 60.f, 10);

		//print positions of all objects
		for (int j = dynamics_world->getNumCollisionObjects() - 1; j >= 0; j--)
		{
			btCollisionObject* obj = dynamics_world->getCollisionObjectArray()[j];
			btRigidBody* body = btRigidBody::upcast(obj);
			btTransform trans;
			if (body && body->getMotionState())
			{
				body->getMotionState()->getWorldTransform(trans);
			}
			else
			{
				trans = obj->getWorldTransform();
			}
			//printf("world pos object %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
		}
	}

	///-----stepsimulation_end-----

	//cleanup in the reverse order of creation/initialization

	///-----cleanup_start-----

	//remove the rigidbodies from the dynamics world and delete them
	for (int i = dynamics_world->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = dynamics_world->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		dynamics_world->removeCollisionObject(obj);
		delete obj;
	}

	//delete collision shapes
	for (int j = 0; j < collision_shapes.size(); j++)
	{
		btCollisionShape* shape = collision_shapes[j];
		collision_shapes[j] = 0;
		delete shape;
	}

    
    // Destroy dynamics world
    SAFE_DELETE(task_scheduler);
    SAFE_DELETE(dynamics_world);
    SAFE_DELETE(solver);
    SAFE_DELETE(solver_pool);
    SAFE_DELETE(overlapping_pair_cache);
    SAFE_DELETE(dispatcher);
    SAFE_DELETE(collision_configuration);

	//next line is optional: it will be cleared by the destructor when the array goes out of scope
	collision_shapes.clear();
}