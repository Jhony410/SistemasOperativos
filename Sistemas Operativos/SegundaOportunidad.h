#ifndef SEGUNDAOPORTUNIDAD_H_INCLUDED
#define SEGUNDAOPORTUNIDAD_H_INCLUDED

#include <vector>
#include <string>
#include <string>
#include "Pagina.h"
using namespace std;

class SegundaOportunidad
{
private:
    vector<Pagina *> marcos;
    int tamanio;
    int puntero;
    int fallos;

public:
    SegundaOportunidad(int tamanio) : tamanio(tamanio), puntero(0), fallos(0)
    {
        marcos.resize(tamanio, nullptr);
    }

    void accederPagina(int numeroPagina)
    {
        cout << "Accediendo a pagina " << numeroPagina << "...\n";

        for (auto &pagina : marcos)
        {
            if (pagina && pagina->numero == numeroPagina)
            {
                cout << "La pagina " << numeroPagina << " ya esta en memoria. Se marca como referenciada.\n";
                pagina->referencia = true;
                return;
            }
        }

        fallos++;
        cout << "Fallos -> " << fallos << endl;

        while (true)
        {
            if (marcos[puntero] == nullptr)
            {
                marcos[puntero] = new Pagina(numeroPagina);
                cout << "Insertando pagina nueva: " << numeroPagina << " en marco vacio [" << puntero << "]\n";
                avanzarPuntero();
                break;
            }
            else if (!marcos[puntero]->referencia)
            {
                cout << "Reemplazando pagina " << marcos[puntero]->numero << " por " << numeroPagina << " en marco [" << puntero << "]\n";
                delete marcos[puntero];
                marcos[puntero] = new Pagina(numeroPagina);
                avanzarPuntero();
                break;
            }
            else
            {
                cout << "Dando segunda oportunidad a pagina " << marcos[puntero]->numero << " en marco [" << puntero << "]\n";
                marcos[puntero]->referencia = false;
                avanzarPuntero();
            }
        }
    }

    void avanzarPuntero()
    {
        puntero = (puntero + 1) % tamanio;
    }

    void mostrarEstado()
    {
        cout << "Memoria: ";
        for (int i = 0; i < tamanio; ++i)
        {
            if (marcos[i])
            {
                cout << "[[" << i << "][" << marcos[i]->numero;
                if (marcos[i]->referencia)
                    cout << "*";
                cout << "]] ";
            }
            else
            {
                cout << "[ Marco [" << i << "] vacio ] ";
            }
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
                if (marcos[i]->referencia)
                    texto += "*";

                GetTextExtentPoint32A(hdc, texto.c_str(), texto.length(), &size);
                int textX = x1 + (ancho - size.cx) / 2;
                int textY = y1 + (alto - size.cy) / 2;

                TextOutA(hdc, textX, textY, texto.c_str(), texto.length());

                const char *mensaje = (marcos[i]->referencia) ? "1° oportunidad" : "2° oportunidad";
                TextOutA(hdc, x1 + 10, y2 + 5, mensaje, strlen(mensaje));
            }
            else
            {
                const char *vacio = "VACIO";
                GetTextExtentPoint32A(hdc, vacio, strlen(vacio), &size);
                int textX = x1 + (ancho - size.cx) / 2;
                int textY = y1 + (alto - size.cy) / 2;

                TextOutA(hdc, textX, textY, vacio, strlen(vacio));
            }

            if (i == puntero)
            {
                TextOutA(hdc, x1 + 35, y1 - 20, "<- P", 4);
            }
        }
    }


    ~SegundaOportunidad()
    {
        for (auto pagina : marcos)
        {
            delete pagina;
        }
    }
};

#endif
