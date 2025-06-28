#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include "resource.h"

#include "SegundaOportunidad.h"
#include "Fifo.h"
#include "Paginacion.h"
#include "Segmento.h"
#include "LRU.h"
#include "MRU.h"
#include "Optimo.h"
#include "NFU.h"

HINSTANCE hInst;
int marcos = 0;

enum AlgoritmoActivo
{
    NINGUNO, FIFO_A, PAGINACION_A, SEGMENTACION_A, LRU_A, MRU_A, OPTIMO_A, NFU_A, SEGUNDA_OPORT_A
} actual = NINGUNO;


SegundaOportunidad* SegundaOportunidadMemoria = nullptr;
FIFO* fifoMemoria = nullptr;
Paginacion* paginacionMemoria = nullptr;
Segmentacion* segmentacionMemoria = nullptr;
LRU* lruMemoria = nullptr;
MRU* mruMemoria = nullptr;
Optimo* optimoMemoria = nullptr;
NFU* nfuMemoria = nullptr;


void destruirAlgoritmos()
{
    delete SegundaOportunidadMemoria;
    SegundaOportunidadMemoria = nullptr;
    delete fifoMemoria;
    fifoMemoria = nullptr;
    delete paginacionMemoria;
    paginacionMemoria = nullptr;
    delete segmentacionMemoria;
    segmentacionMemoria = nullptr;
    delete lruMemoria;
    lruMemoria = nullptr;
    delete mruMemoria;
    mruMemoria = nullptr;
    delete optimoMemoria;
    optimoMemoria = nullptr;
    delete nfuMemoria;
    nfuMemoria = nullptr;
}

void crearAlgoritmo(AlgoritmoActivo nuevo)
{
    destruirAlgoritmos();
    actual = nuevo;
    switch (nuevo)
    {
    case FIFO_A:
        fifoMemoria = new FIFO(marcos);
        break;
    case PAGINACION_A:
        paginacionMemoria = new Paginacion(marcos);
        break;
    case SEGMENTACION_A:
        segmentacionMemoria = new Segmentacion(marcos);
        break;
    case LRU_A:
        lruMemoria = new LRU(marcos);
        break;
    case MRU_A:
        mruMemoria = new MRU(marcos);
        break;
    case OPTIMO_A:
        optimoMemoria = new Optimo(marcos);
        break;
    case NFU_A:
        nfuMemoria = new NFU(marcos);
        break;
    case SEGUNDA_OPORT_A:
        SegundaOportunidadMemoria = new SegundaOportunidad(marcos);
        break;
    default:
        break;
    }
}
void actualizarTextoAlgoritmo(HWND hwndDlg, AlgoritmoActivo actual)
{
    std::string mensaje = "Algoritmo actual: ";
    switch (actual)
    {
    case FIFO_A:
        mensaje += "FIFO";
        break;
    case PAGINACION_A:
        mensaje += "Paginacion";
        break;
    case SEGMENTACION_A:
        mensaje += "Segmentacion";
        break;
    case LRU_A:
        mensaje += "LRU";
        break;
    case MRU_A:
        mensaje += "MRU";
        break;
    case OPTIMO_A:
        mensaje += "Óptimo";
        break;
    case NFU_A:
        mensaje += "NFU";
        break;
    case SEGUNDA_OPORT_A:
        mensaje += "Segunda Oportunidad (Reloj)";
        break;
    default:
        mensaje += "Ninguno";
        break;
    }
    SetDlgItemText(hwndDlg, IDC_ALGORITMO_ACTUAL, mensaje.c_str());
}

BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
        return TRUE;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwndDlg, &ps);
        switch (actual)
        {
        case SEGUNDA_OPORT_A:
            if (SegundaOportunidadMemoria) SegundaOportunidadMemoria->dibujar(hwndDlg, hdc);
            break;
        case FIFO_A:
            if (fifoMemoria) fifoMemoria->dibujar(hwndDlg, hdc);
            break;
        case PAGINACION_A:
            if (paginacionMemoria) paginacionMemoria->dibujar(hwndDlg, hdc);
            break;
        case SEGMENTACION_A:
            if (segmentacionMemoria) segmentacionMemoria->dibujar(hwndDlg, hdc);
            break;
        case LRU_A:
            if (lruMemoria) lruMemoria->dibujar(hwndDlg, hdc);
            break;
        case MRU_A:
            if (mruMemoria) mruMemoria->dibujar(hwndDlg, hdc);
            break;
        case OPTIMO_A:
            if (optimoMemoria) optimoMemoria->dibujar(hwndDlg, hdc);
            break;
        case NFU_A:
            if (nfuMemoria) nfuMemoria->dibujar(hwndDlg, hdc);
            break;
        }
        EndPaint(hwndDlg, &ps);
        return TRUE;
    }

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {

        case BTN_INSERTAR:
        {
            BOOL valido;
            int dato = GetDlgItemInt(hwndDlg, INPUT, &valido, true);
            if (!valido)
            {
                MessageBox(hwndDlg, "Por favor ingrese un numero valido.", "Error", MB_ICONWARNING);
                return TRUE;
            }

            switch (actual)
            {
            case SEGUNDA_OPORT_A:
            {
                SegundaOportunidadMemoria->accederPagina(dato);
                SegundaOportunidadMemoria->mostrarEstado();
                SegundaOportunidadMemoria->mostrarEstadisticas();
            }
            break;
            case FIFO_A:
            {
                fifoMemoria->accederPagina(dato);
                fifoMemoria->mostrarEstado();
                fifoMemoria->mostrarEstadisticas();
            }
            break;
            case PAGINACION_A:
            {
                paginacionMemoria->accederPagina(dato);
                paginacionMemoria->mostrarEstado();
                paginacionMemoria->mostrarEstadisticas();
            }
            break;
            case SEGMENTACION_A:
            {
                segmentacionMemoria->insertarSegmento(dato);
                segmentacionMemoria->mostrarEstado();
                segmentacionMemoria->mostrarEstadisticas();
            }
            break;
            case LRU_A:
            {
                lruMemoria->accederPagina(dato);
                lruMemoria->mostrarEstado();
                lruMemoria->mostrarEstadisticas();
            }
            break;
            case MRU_A:
            {
                mruMemoria->accederPagina(dato);
                mruMemoria->mostrarEstado();
                mruMemoria->mostrarEstadisticas();
            }
            break;
            case OPTIMO_A:
            {
                optimoMemoria->accederPagina(dato);
                optimoMemoria->ejecutarAlgoritmo();
                optimoMemoria->mostrarEstado();
                optimoMemoria->mostrarEstadisticas();
            }
            break;
            case NFU_A:
            {
                nfuMemoria->accederPagina(dato);
                nfuMemoria->mostrarEstado();
                nfuMemoria->mostrarEstadisticas();
            }
            break;
            default:
                MessageBox(hwndDlg, "Seleccione un algoritmo primero.", "Aviso", MB_ICONINFORMATION);
                break;
            }

            InvalidateRect(hwndDlg, nullptr, TRUE);

        }
        return TRUE;

        case BTN_MARCOS:
        {
            BOOL valido;
            int dato = GetDlgItemInt(hwndDlg, EDIT_MARCOS, &valido, TRUE);
            if (valido && dato > 0)
            {
                marcos = dato;
                MessageBox(hwndDlg, "Numero de marcos configurado correctamente.", "Listo", MB_OK);
                string texto = "N° de marcos: " + std::to_string(marcos);
                SetDlgItemTextA(hwndDlg, IDC_NUM_MARCOS, texto.c_str());
            }
            else
            {
                MessageBox(hwndDlg, "Ingrese un número valido de marcos.", "Error", MB_ICONERROR);
            }
            return TRUE;
        }

        case BTN_FIFO:
        {
            crearAlgoritmo(FIFO_A);
            actualizarTextoAlgoritmo(hwndDlg, actual);
            InvalidateRect(hwndDlg, nullptr, true);
        }
        return TRUE;
        case BTN_PAGINACION:
        {
            crearAlgoritmo(PAGINACION_A);
            actualizarTextoAlgoritmo(hwndDlg, actual);
            InvalidateRect(hwndDlg, nullptr, true);
        }
        return TRUE;
        case BTN_SEGMENTACION:
        {
            crearAlgoritmo(SEGMENTACION_A);
            actualizarTextoAlgoritmo(hwndDlg, actual);
            InvalidateRect(hwndDlg, nullptr, true);
        }
        return TRUE;
        case BTN_LRU:
        {
            crearAlgoritmo(LRU_A);
            actualizarTextoAlgoritmo(hwndDlg, actual);
            InvalidateRect(hwndDlg, nullptr, true);
        }
        return TRUE;
        case BTN_MRU:
        {
            crearAlgoritmo(MRU_A);
            actualizarTextoAlgoritmo(hwndDlg, actual);
            InvalidateRect(hwndDlg, nullptr, true);
        }
        return TRUE;
        case BTN_OPTIMO:
        {
            crearAlgoritmo(OPTIMO_A);
            actualizarTextoAlgoritmo(hwndDlg, actual);
            InvalidateRect(hwndDlg, nullptr, true);
        }
        return TRUE;
        case BTN_NFU:
        {
            crearAlgoritmo(NFU_A);
            actualizarTextoAlgoritmo(hwndDlg, actual);
            InvalidateRect(hwndDlg, nullptr, true);
        }
        return TRUE;
        case BTN_SEGUNDA_OPORT:
        {
            crearAlgoritmo(SEGUNDA_OPORT_A);
            actualizarTextoAlgoritmo(hwndDlg, actual);
            InvalidateRect(hwndDlg, nullptr, true);
        }
        return TRUE;
        }
        return TRUE;

    case WM_CLOSE:
    {
        destruirAlgoritmos();
        EndDialog(hwndDlg, 0);
    }
    return TRUE;
    }
    return FALSE;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    hInst = hInstance;
    InitCommonControls();
    return DialogBox(hInst, MAKEINTRESOURCE(DLG_MAIN), nullptr, (DLGPROC)DlgMain);
}
