#define _USE_MATH_DEFINES

#include "math.h"
#include <iostream>

#include <Windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>

#pragma once
const double k = 0.8;
const double min_x = -7;
const double min_y = -7;

class Figure
{
public:
	static double* getTexCoord(double* vertex) {
		double A = vertex[0] < 0
			? vertex[0] * -k
			: (vertex[0] - min_x) * k ;
		double B = vertex[1] < 0
			? vertex[1] * -k
			: (vertex[1] - min_y) * k;
		return new double[] {A, B};
	}

	static double* getVertexNormale(double* vertexA, double* vertexB, double* vertexC, bool isNormalRevert = false) {
		double* A = new double[] {
			vertexB[0] - vertexA[0],
				vertexB[1] - vertexA[1],
				vertexB[2] - vertexA[2]
		};
		double* B = new double[] {
			vertexC[0] - vertexA[0],
				vertexC[1] - vertexA[1],
				vertexC[2] - vertexA[2]
		};

		if (isNormalRevert == false) {
			return new double[] {
				A[1] * B[2] - B[1] * A[2],
					-A[0] * B[2] + B[0] * A[2],
					A[0] * B[1] - B[0] * A[1]
			};
		}
		else {
			return new double[] {
				-(A[1] * B[2] - B[1] * A[2]),
					-(-A[0] * B[2] + B[0] * A[2]),
					-(A[0] * B[1] - B[0] * A[1])
			};
		}
	}

	// Нахождение длины вектора
	static double getLenght(double* A, double* B) {
		return std::sqrt(std::pow(B[0] - A[0], 2) + std::pow(B[1] - A[1], 2));
	}

	// Нахождение центра окружности по 3 точкам
	static double* getCenterCircle(double* vertexA, double* vertexB, double* vertexC) {
		double A, B, C, D, E, F, G;

		A = vertexB[0] - vertexA[0];
		B = vertexB[1] - vertexA[1];
		C = vertexC[0] - vertexA[0];
		D = vertexC[1] - vertexA[1];
		E = A * (vertexA[0] + vertexB[0]) + B * (vertexA[1] + vertexB[1]);
		F = C * (vertexA[0] + vertexC[0]) + D * (vertexA[1] + vertexC[1]);
		G = 2 * (A * (vertexC[1] - vertexB[1]) - B * (vertexC[0] - vertexB[0]));

		return new double[] {
			(D * E - B * F) / G,
				(A * F - C * E) / G,
				vertexA[2]
		};
	}

	// Нахождение точки N для дальнейшего нахождения угла
	static double getAngleY(double y, double y0, double r) {
		return std::asin((y - y0) / r) * 180 / M_PI;
	}

	static double getAngleX(double x, double x0, double r) {
		return std::acos((x - x0) / r) * 180 / M_PI;
	}

	// Нахождение координат точки на окружности
	static double* getVertexCircle(double x, double y, double z, double r, double n) {
		double angle = M_PI * n / 180;
		double dx = r * cos(angle) + x;
		double dy = r * sin(angle) + y;
		double* A = new double[] { dx, dy, z };

		return A;
	}

	// Построение полуокружности по точкам
	static void render_circle(double x, double y, double z, double r, double* baseVertex, double* startVertex, double* endVertex, bool isNormalRevert = false) {
		double start = getAngleX(startVertex[0], x, r);
		double end = getAngleX(endVertex[0], x, r);
		if (start == end) {
			start = getAngleY(startVertex[1], y, r);
			end = getAngleY(endVertex[1], y, r);
		}

		if (start > end) {
			std::swap(start, end);
		}

		for (double i = start; i < end; i += 1)
		{
			double* A = getVertexCircle(x, y, z, r, i);
			double* B = getVertexCircle(x, y, z, r, i + 1);

			glNormal3dv(getVertexNormale(B, baseVertex, A, isNormalRevert));
			glTexCoord2dv(getTexCoord(A));
			glVertex3dv(A);
			glTexCoord2dv(getTexCoord(B));
			glVertex3dv(B);
			glTexCoord2dv(getTexCoord(baseVertex));
			glVertex3dv(baseVertex);
		}
	}

	static void drawRectangleForCircle(double x, double y, double z, double r, double* startVertex, double* endVertex, bool isNormalRevert = false) {
		double start = getAngleX(startVertex[0], x, r);
		double end = getAngleX(endVertex[0], x, r);
		if (start == end) {
			start = getAngleY(startVertex[1], y, r);
			end = getAngleY(endVertex[1], y, r);
		}

		if (start > end) {
			std::swap(start, end);
		}

		for (double i = start; i < end; i += 1)
		{
			double* A = getVertexCircle(x, y, 0, r, i);
			double* B = getVertexCircle(x, y, 0, r, i + 1);
			double* C = getVertexCircle(x, y, z, r, i + 1);
			double* D = getVertexCircle(x, y, z, r, i);

			glNormal3dv(getVertexNormale(A, B, C, isNormalRevert));
			glVertex3dv(A);
			glVertex3dv(B);
			glVertex3dv(C);
			glVertex3dv(D);
		}
	}

	static void figure() {
		double A[] = { 0, 0 ,0 };
		double B[] = { 6, 0, 0 };
		double C[] = { 6,6, 0 };
		double D[] = { -1, 2 ,0 };
		double E[] = { -1, -6, 0 };
		double F[] = { -7,-3, 0 };
		double G[] = { -3, 1 ,0 };
		double H[] = { -7, 6 ,0 };

		double A1[] = { 0, 0 ,5 };
		double B1[] = { 6, 0, 5 };
		double C1[] = { 6,6, 5 };
		double D1[] = { -1, 2 ,5 };
		double E1[] = { -1, -6, 5 };
		double F1[] = { -7,-3, 5 };
		double G1[] = { -3, 1 ,5 };
		double H1[] = { -7, 6 ,5 };

		double M[] = { -3, -4, 0 };
		double M1[] = { -3, -4, 5 };


		glBegin(GL_TRIANGLES);
		render_circle(6, 3, 0, 3, A, B, C, true);

		glNormal3dv(getVertexNormale(D, C, A));
		glTexCoord2dv(getTexCoord(A));
		glVertex3dv(A);
		glTexCoord2dv(getTexCoord(C));
		glVertex3dv(C);
		glTexCoord2dv(getTexCoord(D));
		glVertex3dv(D);

		glNormal3dv(getVertexNormale(D, A, G));
		glTexCoord2dv(getTexCoord(A));
		glVertex3dv(A);
		glTexCoord2dv(getTexCoord(D));
		glVertex3dv(D);
		glTexCoord2dv(getTexCoord(G));
		glVertex3dv(G);

		glNormal3dv(getVertexNormale(D, G, H));
		glTexCoord2dv(getTexCoord(D));
		glVertex3dv(D);
		glTexCoord2dv(getTexCoord(G));
		glVertex3dv(G);
		glTexCoord2dv(getTexCoord(H));
		glVertex3dv(H);
		
		glNormal3dv(getVertexNormale(A, F, G));
		glTexCoord2dv(getTexCoord(A));
		glVertex3dv(A);
		glTexCoord2dv(getTexCoord(G));
		glVertex3dv(G);
		glTexCoord2dv(getTexCoord(F));
		glVertex3dv(F);

		//glNormal3dv(getVertexNormale(A, E, F));
		//glVertex3dv(A);
		//glVertex3dv(F);
		//glVertex3dv(E);
		double* center = getCenterCircle(E, M, F);
		render_circle(center[0], center[1], center[2], getLenght(M, center), A, F, E);

		render_circle(6, 3, 5, 3, A1, B1, C1);

		glNormal3dv(getVertexNormale(D1, A1, C1));
		glTexCoord2dv(getTexCoord(A1));
		glVertex3dv(A1);
		glTexCoord2dv(getTexCoord(C1));
		glVertex3dv(C1);
		glTexCoord2dv(getTexCoord(D1));
		glVertex3dv(D1);

		glNormal3dv(getVertexNormale(D1, G1, A1));
		glTexCoord2dv(getTexCoord(A1));
		glVertex3dv(A1);
		glTexCoord2dv(getTexCoord(D1));
		glVertex3dv(D1);
		glTexCoord2dv(getTexCoord(G1));
		glVertex3dv(G1);

		glNormal3dv(getVertexNormale(D1, H1, G1));
		glTexCoord2dv(getTexCoord(D1));
		glVertex3dv(D1);
		glTexCoord2dv(getTexCoord(G1));
		glVertex3dv(G1);
		glTexCoord2dv(getTexCoord(H1));
		glVertex3dv(H1);

		glNormal3dv(getVertexNormale(A1, G1, F1));
		glTexCoord2dv(getTexCoord(A1));
		glVertex3dv(A1);
		glTexCoord2dv(getTexCoord(G1));
		glVertex3dv(G1);
		glTexCoord2dv(getTexCoord(F1));
		glVertex3dv(F1);

		double* center2 = getCenterCircle(M1, E, F);
		render_circle(center2[0], center2[1], center2[2], getLenght(M1, center2), A1, F, E, true);
		glEnd();

		glBegin(GL_QUADS);
		glNormal3dv(getVertexNormale(B1, A, B));
		glVertex3dv(A);
		glVertex3dv(B);
		glVertex3dv(B1);
		glVertex3dv(A1);

		drawRectangleForCircle(6, 3, 5, 3, B, C);

		glNormal3dv(getVertexNormale(D1, C, D));
		glVertex3dv(C);
		glVertex3dv(D);
		glVertex3dv(D1);
		glVertex3dv(C1);

		glNormal3dv(getVertexNormale(H1, D, H));
		glVertex3dv(D);
		glVertex3dv(H);
		glVertex3dv(H1);
		glVertex3dv(D1);

		glNormal3dv(getVertexNormale(G1, H, G));
		glVertex3dv(H);
		glVertex3dv(G);
		glVertex3dv(G1);
		glVertex3dv(H1);

		glNormal3dv(getVertexNormale(F1, G, F));
		glVertex3dv(G);
		glVertex3dv(F);
		glVertex3dv(F1);
		glVertex3dv(G1);

		drawRectangleForCircle(center[0], center[1], 5, getLenght(M, center), F, E, true);

		glNormal3dv(getVertexNormale(A1, E, A));
		glVertex3dv(E);
		glVertex3dv(A);
		glVertex3dv(A1);
		glVertex3dv(E1);
		glEnd();
	}
};