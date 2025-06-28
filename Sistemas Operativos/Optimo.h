#ifndef OPTIMO_H_INCLUDED
#define OPTIMO_H_INCLUDED

#include <vector>
#include <string>
#include <string>
#include <algorithm>
#include "Pagina.h"
using namespace std;

class Optimo
{
private:
    vector<Pagina *> marcos;
    int tamanio;
    int fallos;
    vector<int> referencias;
    int posicionActual;

public:
    Optimo(int tamanio) : tamanio(tamanio), fallos(0), posicionActual(0)
    {
        marcos.resize(tamanio, nullptr);
    }

    void accederPagina(int numeroPagina)
    {
        referencias.push_back(numeroPagina);
    }

    void ejecutarAlgoritmo()
    {
        fallos = 0;
        for (Pagina *p : marcos)
        {
            delete p;
        }
        fill(marcos.begin(), marcos.end(), nullptr);
        posicionActual = 0;

        for (int i = 0; i < referencias.size(); ++i)
        {
            int pagina = referencias[i];
            cout << "Accediendo a pagina " << pagina << "...\n";

            bool enMemoria = false;
            for (auto &p : marcos)
            {
                if (p && p->numero == pagina)
                {
                    enMemoria = true;
                    break;
                }
            }

            if (enMemoria)
            {
                cout << "La pagina " << pagina << " ya esta en memoria.\n";
            }
            else
            {
                fallos++;
                cout << "Fallo de pagina #" << fallos << "\n";

                bool insertado = false;
                for (int j = 0; j < tamanio; ++j)
                {
                    if (!marcos[j])
                    {
                        marcos[j] = new Pagina(pagina);
                        cout << "Insertado en marco vacio [" << j << "]\n";
                        insertado = true;
                        break;
                    }
                }

                if (!insertado)
                {

                    int maxDistancia = -1;
                    int reemplazarIdx = -1;

                    for (int j = 0; j < tamanio; ++j)
                    {
                        int num = marcos[j]->numero;
                        int distancia = 1e9;

                        for (int k = i + 1; k < referencias.size(); ++k)
                        {
                            if (referencias[k] == num)
                            {
                                distancia = k - i;
                                break;
                            }
                        }

                        if (distancia > maxDistancia)
                        {
                            maxDistancia = distancia;
                            reemplazarIdx = j;
                        }
                    }

                    cout << "Reemplazando pagina " << marcos[reemplazarIdx]->numero << " por " << pagina << " en marco [" << reemplazarIdx << "]\n";
                    delete marcos[reemplazarIdx];
                    marcos[reemplazarIdx] = new Pagina(pagina);
                }
            }
            posicionActual++;
        }

        cout << "TOTAL DE FALLOS: " << fallos << "\n";
    }

    void mostrarEstado()
    {
        cout << "Memoria: ";
        for (int i = 0; i < tamanio; ++i)
        {
            if (marcos[i])
                cout << "[[" << i << "][" << marcos[i]->numero << "]] ";
            else
                cout << "[ Marco [" << i << "] vacio ] ";
        }
        cout << "\n\n";
    }

    void mostrarEstadisticas()
    {
        cout << "Total de fallos de pagina: " << fallos << "\n";
    }

    void dibujar(HWND hwnd, HDC hdc)
    {
        SetBkMode(hdc, TRANSPARENT);
        int ancho = 100;
        int alto = 150;
        int espacioX = 20;
        int espacioY = 40;
        int startX = 50;
        int startY = 90;
        int columnas = 7;

        for (int i = 0; i < tamanio; ++i)
        {
            int fila = i / columnas;
            int columna = i % columnas;

            int x1 = startX + columna * (ancho + espacioX);
            int y1 = startY + fila * (alto + espacioY);
            int x2 = x1 + ancho;
            int y2 = y1 + alto;

            Rectangle(hdc, x1, y1, x2, y2);

            SIZE size;

            if (marcos[i])
            {
                string texto = to_string(marcos[i]->numero);
                GetTextExtentPoint32A(hdc, texto.c_str(), texto.length(), &size);
                int textX = x1 + (ancho - size.cx) / 2;
                int textY = y1 + (alto - size.cy) / 2;
                TextOutA(hdc, textX, textY, texto.c_str(), texto.length());

                const char *mensaje = "OPTIMO";
                TextOutA(hdc, x1 + 20, y2 + 5, mensaje, strlen(mensaje));
            }
            else
            {
                const char *vacio = "VACIO";
                GetTextExtentPoint32A(hdc, vacio, strlen(vacio), &size);
                int textX = x1 + (ancho - size.cx) / 2;
                int textY = y1 + (alto - size.cy) / 2;
                TextOutA(hdc, textX, textY, vacio, strlen(vacio));
            }
        }
    }


    ~Optimo()
    {
        for (auto p : marcos)
            delete p;
    }
};

#endif
