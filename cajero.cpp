#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Usuario {
    string numCuenta;
    string PIN;
    double saldoActual;
    bool tarjetaBloqueada;
    string nombreUsuario;
};

// Prototipos:
Usuario identificarUsuario(const string& numCuenta);
void guardarUsuario(const Usuario& usuario);
bool verificarPIN(const Usuario& usuario, const string& PIN);
void consultarSaldoActual(const Usuario& usuario);
void transferirDinero(Usuario& usuario, double monto);
void depositarDinero(Usuario& usuario, double monto);
void cambiarPin(Usuario& usuario, string& pinCambiado);
void retirarDinero(Usuario& usuario, double monto);
void bloquearTarjeta(Usuario& usuario);
void menuSeleccion(int& opcion);

int main() {
    string numCuenta;
    string PIN;

    cout << "Bienvenido al Cajero Automatico" << endl << endl;
    cout << "Ingrese su numero de cuenta: " << endl;
    cin >> numCuenta;

    Usuario usuario = identificarUsuario(numCuenta);

    if (usuario.numCuenta == numCuenta) {
        if (usuario.tarjetaBloqueada) {
            cout << "La tarjeta esta bloqueada. Comuniquese con su banco para desbloquearla." << endl << endl;
            return 0;
        }

        int intentos = 0;
        bool PINCorrecto = false;
        while (intentos < 3 && !PINCorrecto) {
            cout << "Ingrese su PIN: " << endl;
            cin >> PIN;
            PINCorrecto = verificarPIN(usuario, PIN);
            if (!PINCorrecto) {
                intentos++;
                if (intentos < 3) {
                    cout << "Le quedan " << 3 - intentos << " intentos." << endl;
                } else {
                    bloquearTarjeta(usuario);
                    return 0;
                }
            }
        }

        string pinCambiado;
        int opcion;
        bool descontinuar = false;

        cout << endl;
        cout << "Bienvenido: " << usuario.nombreUsuario << endl;
        cout << endl;

        menuSeleccion(opcion);

        while (opcion != 6 && !descontinuar) {
            switch (opcion) {
                case 1:
                    consultarSaldoActual(usuario);
                    break;
                case 2: 
                    int opcionCaseDos;
                    cout << "¿Seguro desea cambiar su pin de seguridad?" << endl;
                    cout << "Introduzca 1 para cambiarlo o cualquier otro numero para CANCELAR" << endl;
                    cin >> opcionCaseDos;
                    if (opcionCaseDos == 1) {
                        cout << "Ingrese el nuevo pin: ";
                        cin >> pinCambiado;
                        cambiarPin(usuario, pinCambiado);  
                    } else {
                        cout << "Cancelando opcion..." << endl;
                        return 0;
                    } 
                    break;
                case 3:
                    double montoDeposito;
                    cout << "Ingrese el monto a depositar: $";
                    cin >> montoDeposito;
                    depositarDinero(usuario, montoDeposito);
                    break;
                case 4:
                    double montoRetiro;
                    cout << "Ingrese el monto a retirar: $";
                    cin >> montoRetiro;
                    retirarDinero(usuario, montoRetiro);
                    break;
                case 5:
                    double montoTransferencia;
                    transferirDinero(usuario, montoTransferencia);
                    break;
                case 6:
                    cout << "Saliendo..." << endl;
                    return 0;
                    break;
                default:
                    cout << "Opcion invalida. Por favor, seleccione una opcion valida." << endl;
                    break;
            }

            cout << endl; 

            int continua;

            cout << "Ingrese 1 si desea realizar una nueva operacion." << endl;
            cout << "Ingrese cualquier otro numero para finalizar." << endl;
            cin >> continua;

            if (continua == 1) {
                menuSeleccion(opcion);
            } else {
                cout << "Muchas gracias por utilizar nuestros cajeros..." << endl;   
                descontinuar = true;  
            }
        }


          menuSeleccion(opcion); 
         }

    } else {
        cout << "Numero de cuenta invalido" << endl;
    }
  
    return 0;
}

void menuSeleccion(int& opcion) {
    cout << "••••••••••• MENU •••••••••••" << endl;
    cout << "1  Consultar Saldo" << endl;
    cout << "2  Cambiar PIN de seguridad" << endl; 
    cout << "3  Depositar dinero" << endl;
    cout << "4  Retirar dinero" << endl;
    cout << "5  Transferir dinero" << endl;
    cout << "6  Salir" << endl;
    cout << "••••••••••••••••••••••••••••" << endl << endl;

    cout << "Ingrese la opcion deseada: ";
    cin >> opcion;
    cout << endl;
}

// Funcion para leer los datos de un usuario desde un archivo CSV
Usuario identificarUsuario(const string& numCuenta) {
    ifstream archivo("usuarios.csv");
    Usuario usuario;
    usuario.numCuenta = ""; // Inicializar con una cadena vacia
    usuario.tarjetaBloqueada = false;

    if (archivo.is_open()) {
        string cuenta, pin, saldoActual, tarjetaBloqueada, nombreUsuario;
        while (getline(archivo, cuenta, ',') &&
               getline(archivo, pin, ',') &&
               getline(archivo, saldoActual, ',') &&
               getline(archivo, tarjetaBloqueada, ',') &&
               getline(archivo, nombreUsuario)) {
            if (cuenta == numCuenta) {
                usuario.numCuenta = numCuenta; // Asignar el numero de cuenta solo si se encuentra al usuario
                usuario.PIN = pin;
                usuario.saldoActual = stod(saldoActual);
                usuario.tarjetaBloqueada = (tarjetaBloqueada == "1");
                usuario.nombreUsuario = nombreUsuario;
                break;
            }
        }
        archivo.close();
    }
    return usuario;
}

// Funcion para guardar los datos de un usuario en un archivo CSV
void guardarUsuario(const Usuario& usuario) {
    ifstream archivoEntrada("usuarios.csv");
    ofstream archivoSalida("temp.csv");

    if (archivoEntrada.is_open() && archivoSalida.is_open()) {
        string cuenta, pin, saldoActual, tarjetaBloqueada, nombreUsuario;
        while (getline(archivoEntrada, cuenta, ',') &&
               getline(archivoEntrada, pin, ',') &&
               getline(archivoEntrada, saldoActual, ',') &&
               getline(archivoEntrada, tarjetaBloqueada, ',') &&
               getline(archivoEntrada, nombreUsuario)) {
            if (cuenta == usuario.numCuenta) {
                archivoSalida << usuario.numCuenta << ","
                              << usuario.PIN << ","
                              << usuario.saldoActual << ","
                              << (usuario.tarjetaBloqueada ? "1" : "0") << ","
                              << usuario.nombreUsuario << endl;
            } else {
                archivoSalida << cuenta << ","
                              << pin << ","
                              << saldoActual << ","
                              << tarjetaBloqueada << ","
                              << nombreUsuario << endl;
            }
        }
        archivoEntrada.close();
        archivoSalida.close();
        remove("usuarios.csv");
        rename("temp.csv", "usuarios.csv");
    }
}

bool verificarPIN(const Usuario& usuario, const string& PIN) {
    if (usuario.PIN == PIN) {
        return true;
    } else {
        cout << "PIN de seguridad incorrecto" << endl;
        return false;
    }
}

void consultarSaldoActual(const Usuario& usuario) {
    cout << "Saldo actual: $" << usuario.saldoActual << endl;
}

void depositarDinero(Usuario& usuario, double monto) {
    usuario.saldoActual += monto;
    cout << "Se ha depositado $" << monto << " en la cuenta" << endl;
    cout << "Saldo actualizado: $" << usuario.saldoActual << endl;
    guardarUsuario(usuario);
}

void cambiarPin(Usuario& usuario, string& pinCambiado) {
    if(pinCambiado.length() == 4){
        usuario.PIN = pinCambiado;
        cout << "Su pin ha sido cambiado exitosamente" << endl;
        guardarUsuario(usuario);
    } else {
        cout << "Su pin debe tener unicamente 4 digitos" << endl;
    }
}

void retirarDinero(Usuario& usuario, double monto) {
    if (monto <= usuario.saldoActual) {
        usuario.saldoActual -= monto;
        cout << "Se ha retirado $" << monto << " de la cuenta" << endl;
        cout << "Saldo Actualizado: $" << usuario.saldoActual << endl;
        guardarUsuario(usuario);
    } else {
        cout << "Saldo insuficiente" << endl;
    }
}

void transferirDinero(Usuario& usuario, double monto) {
    string numCuentaDestino;
    cout << "Ingrese el numero de cuenta destino: ";
    cin >> numCuentaDestino;
            
    if (monto <= usuario.saldoActual) {
        Usuario usuarioDestino = identificarUsuario(numCuentaDestino);

        if(usuarioDestino.numCuenta != "" && !usuarioDestino.tarjetaBloqueada && usuarioDestino.numCuenta != usuario.numCuenta) {
            cout << "Ingrese el monto a transferir: $";
            cin >> monto;
            usuario.saldoActual -= monto;
            usuarioDestino.saldoActual += monto;
            cout << "Se ha transferido $" << monto << " a la cuenta " << numCuentaDestino << endl;
            cout << "Saldo actual: $" << usuario.saldoActual << endl;
            guardarUsuario(usuario);
            guardarUsuario(usuarioDestino);
        } else {
            cout << "Error, Usuario inexistente o bloqueado." << endl;
        }    
    } else {
        cout << "Saldo insuficiente" << endl;
    }
}

void bloquearTarjeta(Usuario& usuario) {
    usuario.tarjetaBloqueada = true;
    guardarUsuario(usuario);
    cout << "Tarjeta bloqueada, comuniquese al numero del banco 0800-232-1030 para desbloquearla." << endl << endl;
}