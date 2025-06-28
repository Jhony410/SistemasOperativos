#ifndef MRU_H_INCLUDED
#define MRU_H_INCLUDED

#include <vector>
#include <string>
#include <string>
#include "Pagina.h"
using namespace std;

class MRU
{
private:
    vector<Pagina *> marcos;
    vector<int> ordenUso;
    int tamanio;
    int fallos;

public:
    MRU(int tamanio) : tamanio(tamanio), fallos(0)
    {
        marcos.resize(tamanio, nullptr);
    }

    void accederPagina(int numeroPagina)
    {
        cout << "Accediendo a pagina " << numeroPagina << "...\n";

        for (int i = 0; i < tamanio; ++i)
        {
            if (marcos[i] && marcos[i]->numero == numeroPagina)
            {
                ordenUso.erase(remove(ordenUso.begin(), ordenUso.end(), numeroPagina), ordenUso.end());
                ordenUso.push_back(numeroPagina);
                cout << "La pagina " << numeroPagina << " ya esta en memoria. Se marca como mas reciente.\n";
                return;
            }
        }

        fallos++;
        cout << "Fallos -> " << fallos << endl;

        for (int i = 0; i < tamanio; ++i)
        {
            if (!marcos[i])
            {
                marcos[i] = new Pagina(numeroPagina);
                ordenUso.push_back(numeroPagina);
                cout << "Insertando pagina " << numeroPagina << " en marco vacio [" << i << "]\n";
                return;
            }
        }

        int mruNumero = ordenUso.back();
        ordenUso.pop_back();

        for (int i = 0; i < tamanio; ++i)
        {
            if (marcos[i] && marcos[i]->numero == mruNumero)
            {
                cout << "Reemplazando MRU " << mruNumero << " por " << numeroPagina << " en marco [" << i << "]\n";
                delete marcos[i];
                marcos[i] = new Pagina(numeroPagina);
                break;
            }
        }

        ordenUso.push_back(numeroPagina);
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

                const char *mensaje = "MRU";
                TextOutA(hdc, x1 + 35, y2 + 5, mensaje, strlen(mensaje));
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


    ~MRU()
    {
        for (auto p : marcos)
            delete p;
    }
};

#endif
