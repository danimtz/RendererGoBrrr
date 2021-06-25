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
	float m_fov;


	int m_cam_type; //0 free move, 1 orbit/spherical

	//Spherical camera
	Vec3f m_sph_coords; //r, theta, phi
	Vec3f m_origin; //Spherical coordinate origin

	float camSpeed{0.1f};

	Mat4f m_projection_mat;//CREATE THIS


	//Constructor
	Camera(Vec3f pos = Vec3f(0.0f, 0.0f, 0.0f),  float pitch = D_PITCH, float yaw = D_YAW, Vec3f up = Vec3f(0.0f, 1.0f, 0.0f)) : m_pos(pos), m_yaw(yaw), 
																																	m_world_up(up), m_pitch(pitch), m_fov(45.0f), m_cam_type(1)
	{
		//FUSTRUM PARAMETERS ON A STRUCT
		m_projection_mat = Mat4f::createProjectionMat(m_fov);

		m_sph_coords.x = Vec3f::distance(m_pos, m_origin);

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
		if (m_cam_type == 1)
		{
			updateSphericalCam();
		}
		else if (m_cam_type == 0)
		{
			updateFreeCam();
		}
	}
	
	void updateFreeCam()
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

	void updateSphericalCam()
	{
		//Todo
		m_sph_coords.y = -((90.0+m_yaw) * (M_PI / 180)); //theta
		m_sph_coords.z = (90.0-m_pitch) * (M_PI / 180); //phi  0 pitch is 90 degrees phi

		float r = m_sph_coords.x;
		float theta = m_sph_coords.y;
		float phi = m_sph_coords.z;

		//calculate pos in cartesian
		m_pos.z = r * std::cos(theta) * std::sin(phi);
		m_pos.x = r * std::sin(theta) * std::sin(phi);
		m_pos.y = r * std::cos(phi);
		
		m_front = m_origin - m_pos;
		m_front.normalize();

		m_right = m_front.cross(m_world_up);
		m_right.normalize();

		m_up = m_right.cross(m_front);
		m_up.normalize();
	}

	void updateCamFOV(float new_fov)
	{	
		m_fov = new_fov;
		m_projection_mat = Mat4f::createProjectionMat(m_fov);
	}

	void changeCamType() 
	{

		resetCam();
		if (m_cam_type == 0)
		{
			m_cam_type = 1;
		}
		else if (m_cam_type == 1)
		{
			m_cam_type = 0;
		}
		updateCam();
	}

	void resetCam() 
	{
		m_pos =	Vec3f(0, 0, 2);
		m_front = Vec3f(0, 0, -1);
		m_right = m_front.cross(m_world_up);
		m_right.normalize();
		
		m_pitch = D_PITCH;
		m_yaw = D_YAW;
		
		//Reset spherical coordinates
		m_sph_coords.x = 2; //some missing
		m_origin = Vec3f(0);
	}
	
};