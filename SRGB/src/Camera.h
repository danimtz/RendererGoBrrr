#pragma once

#include "Vector3.h"
#include "Matrix4.h"
#include "SDL.h"

//Default cam constants
const float D_PITCH = 0.0f;
const float D_YAW = -90.0f;




class Camera {
public:
	
	//Member data
	Vec3f m_pos;
	Vec3f m_up;
	Vec3f m_right;
	Vec3f m_world_up;
	Vec3f m_front;

	float m_yaw;
	float m_pitch;

	float camSpeed{0.1f};

	Mat4f m_projection_mat;//CREATE THIS


	//Constructor
	Camera(Vec3f pos = Vec3f(0.0f, 0.0f, 0.0f),  float pitch = D_PITCH, float yaw = D_YAW, Vec3f up = Vec3f(0.0f, 1.0f, 0.0f)) : m_pos(pos), m_yaw(yaw), 
																																	m_world_up(up), m_pitch(pitch)
	{
		//FUSTRUM PARAMETERS ON A STRUCT
		m_projection_mat = Mat4f::createProjectionMat();
		updateCam();
	}
	
	Mat4f getViewMat()
	{
		return Mat4f::createLookAt(m_pos, m_pos+m_front, m_up);
	}

	Mat4f getProjectionMat() const
	{
		return m_projection_mat;
	}


	
	void updateCam()
	{
		//New front vector
		
		float radYaw = m_yaw*(M_PI / 180);
		float radPitch = m_pitch*(M_PI / 180);

		m_front.x = std::cos(radYaw) * std::cos(radPitch);
		m_front.y = std::sin(radPitch);
		m_front.z = std::sin(radYaw) * std::cos(radPitch);

		m_front.normalize();

		m_right = m_front.cross(m_world_up);
		m_right.normalize();

		m_up = m_right.cross(m_front);
		m_up.normalize();
		
	}
};