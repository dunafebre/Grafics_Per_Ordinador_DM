#include "camera.h"
#include <cmath>
#include "main/includes.h"
#include <iostream>

Camera::Camera()
{
	view_matrix.SetIdentity();
	SetOrthographic(-1,1,1,-1,-1,1);
}

Vector3 Camera::GetLocalVector(const Vector3& v)
{
	Matrix44 iV = view_matrix;
	if (iV.Inverse() == false)
		std::cout << "Matrix Inverse error" << std::endl;
	Vector3 result = iV.RotateVector(v);
	return result;
}

Vector3 Camera::ProjectVector(Vector3 pos)
{
	Vector4 pos4 = Vector4(pos.x, pos.y, pos.z, 1.0);
	Vector4 result = viewprojection_matrix * pos4;
	if (type == ORTHOGRAPHIC)
		return result.GetVector3();
	else
		return result.GetVector3() / result.w;
}

void Camera::Rotate(float angle, const Vector3& axis)
{
	Matrix44 R;
	R.MakeRotationMatrix(angle, axis);
	Vector3 new_front = R * (center - eye);
	center = eye + new_front;
	UpdateViewMatrix();
}

void Camera::Move(Vector3 delta)
{
	Vector3 localDelta = GetLocalVector(delta);
	eye = eye - localDelta;
	center = center - localDelta;
	UpdateViewMatrix();
}

void Camera::SetOrthographic(float left, float right, float top, float bottom, float near_plane, float far_plane)
{
	type = ORTHOGRAPHIC;

	this->left = left;
	this->right = right;
	this->top = top;
	this->bottom = bottom;
	this->near_plane = near_plane;
	this->far_plane = far_plane;

	UpdateProjectionMatrix();
}

void Camera::SetPerspective(float fov, float aspect, float near_plane, float far_plane)
{
	type = PERSPECTIVE;

	this->fov = fov;
	this->aspect = aspect;
	this->near_plane = near_plane;
	this->far_plane = far_plane;

	UpdateProjectionMatrix();
}

void Camera::LookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
{
	this->eye = eye;
	this->center = center;
	this->up = up;

	UpdateViewMatrix();
}

void Camera::UpdateViewMatrix()
{
	// Reset Matrix (Identity)
	view_matrix.SetIdentity();

	// Comment this line to create your own projection matrix!
	//SetExampleViewMatrix();

    Matrix44 rotation;
    Matrix44 translation;
    
    rotation.SetIdentity();
    translation.SetIdentity();
    
    Vector3 z_cam = center - eye;
    z_cam.Normalize();
    Vector3 x_cam = up.Cross(z_cam);
    x_cam.Normalize();
    Vector3 y_cam = z_cam.Cross(x_cam);
    
    rotation.M[0][0] = x_cam.x;
    rotation.M[0][1] = x_cam.y;
    rotation.M[0][2] = x_cam.z;

    rotation.M[1][0] = y_cam.x;
    rotation.M[1][1] = y_cam.y;
    rotation.M[1][2] = y_cam.z;

    rotation.M[2][0] = -z_cam.x;
    rotation.M[2][1] = -z_cam.y;
    rotation.M[2][2] = -z_cam.z;
    
    rotation.M[3][3] = 1.0f;
    
    translation.MakeTranslationMatrix(-eye.x, -eye.y, -eye.z);

    //view = R⁻¹ · T⁻¹
    view_matrix = rotation * translation;

	UpdateViewProjectionMatrix();
}

// Create a projection matrix
void Camera::UpdateProjectionMatrix()
{
	// Reset Matrix (Identity)
	projection_matrix.SetIdentity();

	// Comment this line to create your own projection matrix!
	//SetExampleProjectionMatrix();
	
	if (type == PERSPECTIVE) {
        float fov_rad = fov * DEG2RAD;
        float tan_half_fov = tan(fov_rad / 2.0f);
        float n = near_plane;
        float f = far_plane;

        projection_matrix.M[0][0] = 1.0f / (tan_half_fov * aspect);
        projection_matrix.M[1][1] = 1.0f / tan_half_fov;
        projection_matrix.M[2][2] = (f + n) / (n - f);
        projection_matrix.M[2][3] = -1.0f;
        projection_matrix.M[3][2] = (2.0f * f * n) / (n - f);
        projection_matrix.M[3][3] = 0.0f;
		
	}
	else if (type == ORTHOGRAPHIC) {
        float l = left, r = right;
        float b = bottom, t = top;
        float n = near_plane, f = far_plane;

        projection_matrix.M[0][0] = 2.0f / (r - l);
        projection_matrix.M[1][1] = 2.0f / (t - b);
        projection_matrix.M[2][2] = 2.0f / (n - f);

        projection_matrix.M[3][0] = -(r + l) / (r - l);
        projection_matrix.M[3][1] = -(t + b) / (t - b);
        projection_matrix.M[3][2] = -(n + f) / (n - f);

        projection_matrix.M[3][3] = 1.0f;
	}

	UpdateViewProjectionMatrix();
}

void Camera::UpdateViewProjectionMatrix()
{
	viewprojection_matrix = projection_matrix * view_matrix;
}

Matrix44 Camera::GetViewProjectionMatrix()
{
	UpdateViewMatrix();
	UpdateProjectionMatrix();

	return viewprojection_matrix;
}

// The following methods have been created for testing.
// Do not modify them.

void Camera::SetExampleViewMatrix()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z);
	glGetFloatv(GL_MODELVIEW_MATRIX, view_matrix.m );
}

void Camera::SetExampleProjectionMatrix()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (type == PERSPECTIVE)
		gluPerspective(fov, aspect, near_plane, far_plane);
	else
		glOrtho(left,right,bottom,top,near_plane,far_plane);

	glGetFloatv(GL_PROJECTION_MATRIX, projection_matrix.m );
	glMatrixMode(GL_MODELVIEW);
}
