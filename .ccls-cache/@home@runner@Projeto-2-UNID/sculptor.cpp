#include "sculptor.hpp"
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;
/*! \brief
 * Construtor da classe
 */ 
Sculptor::Sculptor(int _nx, int _ny, int _nz) {
  nx = _nx;
  ny = _ny;
  nz = _nz;
  r = 1;
  g = 1;
  b = 1;
  v = new Voxel **[nx];

  for (int i = 0; i < nx; i++) {
    v[i] = new Voxel *[ny];

    for (int j = 0; j < ny; j++) {
      v[i][j] = new Voxel[nz];
    }
  }
}

/*! \brief
 * Destrutor da classe
 */
Sculptor::~Sculptor() {
  for (int i = 0; i < nx; i++) {
    for (int j = 0; j < ny; j++) {
      delete[] v[i][j];
    }
  }

  for (int i = 0; i < nx; i++) {
    delete[] v[i];
  }

  delete[] v;
  nx = 0;
  ny = 0;
  nz = 0;
}

/*! \brief
 * Define a cor atual de desenho.
 */
void Sculptor::setColor(float r, float g, float b, float a) {
  Sculptor::r = r;
  Sculptor::g = g;
  Sculptor::b = b;
  Sculptor::a = a;
}

/*! \brief
 * Ativa o voxel na posição (x,y,z) (fazendo show = true) e atribui ao mesmo a cor atual de desenho
 */
void Sculptor::putVoxel(int x, int y, int z) {
  v[x][y][z].show = true;
  v[x][y][z].r = r;
  v[x][y][z].g = g;
  v[x][y][z].b = b;
  v[x][y][z].a = a;
}

/*! \brief
 * Desativa o voxel na posição (x,y,z) (fazendo show = false)
 */
void Sculptor::cutVoxel(int x, int y, int z) { v[x][y][z].show = false; }

/*! \brief
 * Ativa todos os voxels no intervalo x∈[x0,x1], y∈[y0,y1], z∈[z0,z1] e atribui aos mesmos a cor atual de desenho
 */
void Sculptor::putBox(int x0, int x1, int y0, int y1, int z0, int z1) {
  for (int x = x0; x < x1; x++) {
    for (int y = y0; y < y1; y++) {
      for (int z = z0; z < z1; z++) {
        v[x][y][z].show = true;
        v[x][y][z].r = r;
        v[x][y][z].g = g;
        v[x][y][z].b = b;
        v[x][y][z].a = a;
      }
    }
  }
}

/*! \brief
 * Desativa todos os voxels no intervalo x∈[x0,x1], y∈[y0,y1], z∈[z0,z1]
 */
void Sculptor::cutBox(int x0, int x1, int y0, int y1, int z0, int z1) {
  for (int x = x0; x < x1; x++) {
    for (int y = y0; y < y1; y++) {
      for (int z = z0; z < z1; z++) {
        v[x][y][z].show = false;
      }
    }
  }
}
/*! \brief
 *Ativa todos os voxels que satisfazem à equação da esfera e atribui aos mesmos a cor atual de desenho (r,g,b,a)
 */
void Sculptor::putSphere(int xcenter, int ycenter, int zcenter, int radius) {
  int i, j, k;
  for (i = 0; i < nx; i++) {
    for (j = 0; j < ny; j++) {
      for (k = 0; k < nz; k++) {

        if ((pow(i-xcenter,2) + pow(j-ycenter,2) + pow(k-zcenter,2)) < radius * radius) {
          Sculptor::putVoxel(i, j, k);
        }
      }
    }
  }
}
/*! \brief
 * Desativa todos os voxels que satisfazem à equação da esfera
 */
void Sculptor::cutSphere(int xcenter, int ycenter, int zcenter, int radius) {
  int i, j, k;
  for (i = 0; i < nx; i++) {
    for (j = 0; j < ny; j++) {
      for (k = 0; k < nz; k++) {
        if ((pow(i -xcenter,2) + pow(j-ycenter,2) + (k -zcenter) < radius * radius)) {
          Sculptor::cutVoxel (i, j, k);
        }
      }
    }
  }
}
/*! \brief
 * Ativa todos os voxels que satisfazem à equação do elipsóide e atribui aos mesmos a cor atual de desenho
 */
void Sculptor::putEllipsoid(int xcenter, int ycenter, int zcenter, int rx,
                            int ry, int rz) {
  float newx, newy, newz;
  int x, y, z;
  for (x = 0; x < nx; x++) {
    for (y = 0; y < ny; y++) {
      for (z = 0; z < nz; z++) {
        newx = ((float)(x - xcenter) * (float)(x - xcenter)) / (rx * rx);
        newy = ((float)(y - ycenter) * (float)(y - ycenter)) / (ry * ry);
        newz = ((float)(z - zcenter) * (float)(z - zcenter)) / (rz * rz);
        if ((newx + newy + newz) < 1) {
          Sculptor::putVoxel(x, y, z);
        }
      }
    }
  }
}
/*! \brief
 * Desativa todos os voxels que satisfazem à equação do elipsóide
 */
void Sculptor::cutEllipsoid(int xcenter, int ycenter, int zcenter, int rx,
                            int ry, int rz) {
  double newx, newy, newz;
  int x, y, z;
  for (x = 0; x < nx; x++) {
    for (y = 0; y < ny; y++) {
      for (z = 0; z < nz; z++) {
        newx = ((float)(x - xcenter) * (float)(x - xcenter)) / (rx * rx);
        newy = ((float)(y - ycenter) * (float)(y - ycenter)) / (ry * ry);
        newz = ((float)(z - zcenter) * (float)(z - zcenter)) / (rz * rz);
        if ((newx + newy + newz) < 1) {
          Sculptor::cutVoxel(x, y, z);
        }
      }
    }
  }
}
/*! \brief
 * grava a escultura no formato OFF no arquivo filename
 */
void Sculptor::writeOFF(const char *filename) {
  int qtd_Voxel = 0;
  int ref;
  int i, j, k, a, b, c;
  float fix = 0.5;
  std::ofstream endArq;
  endArq.open(filename);
  if (endArq.is_open()) {
    endArq << "OFF\n";

    for (i = 0; i < nx; i++) {
      for (j = 0; j < ny; j++) {
        for (k = 0; k < nz; k++) {
          if (v[i][j][k].show == true) {
            qtd_Voxel++;
          }
        }
      }
    }

    endArq << qtd_Voxel * 8 << " " << qtd_Voxel * 6 << " "
           << "0"
           << "\n";

    for (a = 0; a < nx; a++) {
      for (b = 0; b < ny; b++) {
        for (c = 0; c < nz; c++) {
          if (v[a][b][c].show == true) {

            endArq << a - fix << " " << b + fix << " " << c - fix << "\n"
                   << flush;
            endArq << a - fix << " " << b - fix << " " << c - fix << "\n"
                   << flush;
            endArq << a + fix << " " << b - fix << " " << c - fix << "\n"
                   << flush;
            endArq << a + fix << " " << b + fix << " " << c - fix << "\n"
                   << flush;
            endArq << a - fix << " " << b + fix << " " << c + fix << "\n"
                   << flush;
            endArq << a - fix << " " << b - fix << " " << c + fix << "\n"
                   << flush;
            endArq << a + fix << " " << b - fix << " " << c + fix << "\n"
                   << flush;
            endArq << a + fix << " " << b + fix << " " << c + fix << "\n"
                   << flush;
          }
        }
      }
    }

    qtd_Voxel = 0;
    for (a = 0; a < nx; a++) {
      for (b = 0; b < ny; b++) {
        for (c = 0; c < nz; c++) {
          if (v[a][b][c].show == true) {
            ref = qtd_Voxel * 8;
            endArq << fixed;

            endArq << "4"
                   << " " << 0 + ref << " " << 3 + ref << " " << 2 + ref << " "
                   << 1 + ref << " ";
            endArq << setprecision(2) << v[a][b][c].r << " " << setprecision(2)
                   << v[a][b][c].g << " " << setprecision(2) << v[a][b][c].b
                   << " " << setprecision(2) << v[a][b][c].a << "\n";

            endArq << "4"
                   << " " << 4 + ref << " " << 5 + ref << " " << 6 + ref << " "
                   << 7 + ref << " ";
            endArq << setprecision(2) << v[a][b][c].r << " " << setprecision(2)
                   << v[a][b][c].g << " " << setprecision(2) << v[a][b][c].b
                   << " " << setprecision(2) << v[a][b][c].a << "\n";

            endArq << "4"
                   << " " << 0 + ref << " " << 1 + ref << " " << 5 + ref << " "
                   << 4 + ref << " ";
            endArq << setprecision(2) << v[a][b][c].r << " " << setprecision(2)
                   << v[a][b][c].g << " " << setprecision(2) << v[a][b][c].b
                   << " " << setprecision(2) << v[a][b][c].a << "\n";

            endArq << "4"
                   << " " << 0 + ref << " " << 4 + ref << " " << 7 + ref << " "
                   << 3 + ref << " ";
            endArq << setprecision(2) << v[a][b][c].r << " " << setprecision(2)
                   << v[a][b][c].g << " " << setprecision(2) << v[a][b][c].b
                   << " " << setprecision(2) << v[a][b][c].a << "\n";

            endArq << "4"
                   << " " << 3 + ref << " " << 7 + ref << " " << 6 + ref << " "
                   << 2 + ref << " ";
            endArq << setprecision(2) << v[a][b][c].r << " " << setprecision(2)
                   << v[a][b][c].g << " " << setprecision(2) << v[a][b][c].b
                   << " " << setprecision(2) << v[a][b][c].a << "\n";

            endArq << "4"
                   << " " << 1 + ref << " " << 2 + ref << " " << 6 + ref << " "
                   << 5 + ref << " ";
            endArq << setprecision(2) << v[a][b][c].r << " " << setprecision(2)
                   << v[a][b][c].g << " " << setprecision(2) << v[a][b][c].b
                   << " " << setprecision(2) << v[a][b][c].a << "\n";
            qtd_Voxel++;
          }
        }
      }
    }
  }

  else {
    cout << "Erro ao abrir arquivo de texto.";
  }
  endArq.close();
}