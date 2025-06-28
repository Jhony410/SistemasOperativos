#ifndef SEGMENTO_H_INCLUDED
#define SEGMENTO_H_INCLUDED

#include <vector>
#include <string>
#include <string>
#include "Pagina.h"
using namespace std;

class Segmento
{
public:
    int id;
    int tamanio;
    Segmento(int id, int tamanio) : id(id), tamanio(tamanio) {}
};

class Segmentacion
{
private:
    vector<Segmento *> memoria;
    int capacidadTotal;
    int usado;
    int idActual;
    int fallos;

public:
    Segmentacion(int capacidad) : capacidadTotal(capacidad), usado(0), idActual(1), fallos(0) {}

    void insertarSegmento(int tamanioSeg)
    {

        if (tamanioSeg <= 0)
        {
            cout << "Error: El tamanio del segmento debe ser mayor a 0.\n";
            return;
        }

        if (tamanioSeg > capacidadTotal)
        {
            cout << "Error: El tamanio del segmento excede la capacidad total de memoria.\n";
            fallos++;
            return;
        }

        cout << "Solicitando insertar segmento de tamanio " << tamanioSeg << "...\n";
        if (usado + tamanioSeg <= capacidadTotal)
        {
            memoria.push_back(new Segmento(idActual++, tamanioSeg));
            usado += tamanioSeg;
            cout << "Segmento insertado. Memoria usada: " << usado << "/" << capacidadTotal << "\n";
        }
        else
        {
            cout << "¡No hay suficiente espacio! Fallo de asignacion.\n";
            fallos++;
        }
    }

    void mostrarEstado()
    {
        cout << "Memoria segmentada:\n";
        for (auto s : memoria)
        {
            cout << "[ID: " << s->id << ", Tam: " << s->tamanio << "] ";
        }
        cout << "\nUsado: " << usado << " / " << capacidadTotal << "\n\n";
    }

    void mostrarEstadisticas()
    {
        cout << "Fallos de asignacion de segmento: " << fallos << "\n";
    }

    void dibujar(HWND hwnd, HDC hdc)
    {
        SetBkMode(hdc, TRANSPARENT);
        int totalAlto = 550;
        int ancho = 450;

        int startX = 260;
        int startY = 90;

        int yActual = startY;

        for (auto s : memoria)
        {
            int h = (s->tamanio * totalAlto) / capacidadTotal;
            Rectangle(hdc, startX, yActual, startX + ancho, yActual + h);

            string texto = "ID " + to_string(s->id) + " (" + to_string(s->tamanio) + ")";
            SIZE size;
            GetTextExtentPoint32A(hdc, texto.c_str(), texto.length(), &size);
            int textX = startX + (ancho - size.cx) / 2;
            int textY = yActual + (h - size.cy) / 2;
            TextOutA(hdc, textX, textY, texto.c_str(), texto.length());

            yActual += h;
        }

        if (usado < capacidadTotal)
        {
            int libre = totalAlto - ((usado * totalAlto) / capacidadTotal);
            Rectangle(hdc, startX, yActual, startX + ancho, yActual + libre);

            const char *vacio = "ESPACIO LIBRE";
            SIZE size;
            GetTextExtentPoint32A(hdc, vacio, strlen(vacio), &size);
            int textX = startX + (ancho - size.cx) / 2;
            int textY = yActual + (libre - size.cy) / 2;
            TextOutA(hdc, textX, textY, vacio, strlen(vacio));
        }
    }

    ~Segmentacion()
    {
        for (auto s : memoria)
            delete s;
    }
};

#endif
