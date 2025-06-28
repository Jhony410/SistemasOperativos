#ifndef NFU_H_INCLUDED
#define NFU_H_INCLUDED

#include <vector>
#include <string>
#include <string>
#include <algorithm>
#include "Pagina.h"
using namespace std;

class PaginaNFU
{
public:
    int numero;
    int contador;

    PaginaNFU(int numero) : numero(numero), contador(1) {}
};

class NFU
{
private:
    vector<PaginaNFU *> marcos;
    int tamanio;
    int fallos;

public:
    NFU(int tamanio) : tamanio(tamanio), fallos(0)
    {
        marcos.resize(tamanio, nullptr);
    }

    void accederPagina(int numeroPagina)
    {
        cout << "Accediendo a pagina " << numeroPagina << "...\n";

        for (auto &p : marcos)
        {
            if (p && p->numero == numeroPagina)
            {
                p->contador++;
                cout << "Pagina " << numeroPagina << " ya en memoria. Contador: " << p->contador << "\n";
                return;
            }
        }

        fallos++;
        cout << "Fallos -> " << fallos << endl;

        for (int i = 0; i < tamanio; ++i)
        {
            if (!marcos[i])
            {
                marcos[i] = new PaginaNFU(numeroPagina);
                cout << "Insertando pagina nueva " << numeroPagina << " en marco vacio [" << i << "]\n";
                return;
            }
        }

        int minCont = 1e9, reemplazo = -1;
        for (int i = 0; i < tamanio; ++i)
        {
            if (marcos[i]->contador < minCont)
            {
                minCont = marcos[i]->contador;
                reemplazo = i;
            }
        }

        cout << "Reemplazando pagina " << marcos[reemplazo]->numero << " (cont=" << minCont << ") por " << numeroPagina << "\n";
        delete marcos[reemplazo];
        marcos[reemplazo] = new PaginaNFU(numeroPagina);
    }

    void mostrarEstado()
    {
        cout << "Memoria: ";
        for (int i = 0; i < tamanio; ++i)
        {
            if (marcos[i])
                cout << "[[" << i << "][" << marcos[i]->numero << "|c:" << marcos[i]->contador << "]] ";
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
                string texto = to_string(marcos[i]->numero) + " (c:" + to_string(marcos[i]->contador) + ")";
                GetTextExtentPoint32A(hdc, texto.c_str(), texto.length(), &size);
                int textX = x1 + (ancho - size.cx) / 2;
                int textY = y1 + (alto - size.cy) / 2;

                TextOutA(hdc, textX, textY, texto.c_str(), texto.length());

                const char *mensaje = "NFU";
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

    ~NFU()
    {
        for (auto p : marcos)
            delete p;
    }
};

#endif
