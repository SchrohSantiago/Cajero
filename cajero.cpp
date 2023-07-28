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


// Función para leer los datos de un usuario desde un archivo CSV
Usuario identificarUsuario(const string& numCuenta) {
    ifstream archivo("usuarios.csv");
    Usuario usuario;
    usuario.numCuenta = numCuenta;
    usuario.tarjetaBloqueada = false;

    if (archivo.is_open()) {
        string cuenta, pin, saldoActual, tarjetaBloqueada, nombreUsuario;
        while (getline(archivo, cuenta, ',') &&
               getline(archivo, pin, ',') &&
               getline(archivo, saldoActual, ',') &&
               getline(archivo, tarjetaBloqueada, ',') &&
               getline(archivo, nombreUsuario)) {
            if (cuenta == numCuenta) {
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


// Función para guardar los datos de un usuario en un archivo CSV
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

// Función para verificar el PIN ingresado por el usuario
bool verificarPIN(const Usuario& usuario, const string& PIN) {
    if (usuario.PIN == PIN) {
        return true;
    } else {
        cout << "PIN incorrecto." << endl;
        return false;
    }
}

// Función para consultar el saldoActual de la cuenta
void consultarsaldoActual(const Usuario& usuario) {
    cout << "saldoActual actual: $" << usuario.saldoActual << endl;
}

// Función para depositar dinero en la cuenta
void depositarDinero(Usuario& usuario, double monto) {
    usuario.saldoActual += monto;
    cout << "Se ha depositado $" << monto << " en la cuenta." << endl;
    cout << "saldoActual actual: $" << usuario.saldoActual << endl;
    guardarUsuario(usuario);
}

// Cambiar pin de seguridad

void cambiarPin(Usuario& usuario, string& pinCambiado) {
    if(pinCambiado.length() == 4){
    usuario.PIN = pinCambiado;
    cout << "Su pin a cambiado exitosamente";
    guardarUsuario(usuario);
    } else {
        cout << "Su pin debe tener unicamente 4 digitos";
    }
}

// Función para retirar dinero de la cuenta
void retirarDinero(Usuario& usuario, double monto) {
    if (monto <= usuario.saldoActual) {
        usuario.saldoActual -= monto;
        cout << "Se ha retirado $" << monto << " de la cuenta." << endl;
        cout << "saldoActual actual: $" << usuario.saldoActual << endl;
        guardarUsuario(usuario);
    } else {
        cout << "saldoActual insuficiente." << endl;
    }
}

// Función para transferir dinero a otra cuenta
void transferirDinero(Usuario& usuario, const string& numCuentaDestino, double monto) {
    if (monto <= usuario.saldoActual) {
        Usuario usuarioDestino = identificarUsuario(numCuentaDestino);
        if (usuarioDestino.numCuenta == numCuentaDestino && !usuarioDestino.tarjetaBloqueada) {
            usuario.saldoActual -= monto;
            usuarioDestino.saldoActual += monto;
            cout << "Se ha transferido $" << monto << " a la cuenta " << numCuentaDestino << "." << endl;
            cout << "saldoActual actual: $" << usuario.saldoActual << endl;
            guardarUsuario(usuario);
            guardarUsuario(usuarioDestino);
        } else {
            cout << "La cuenta destino no existe o está tarjetaBloqueada." << endl;
        }
    } else {
        cout << "saldoActual insuficiente." << endl;
    }
}

// Función para bloquear la tarjeta del usuario
void bloquearTarjeta(Usuario& usuario) {
    usuario.tarjetaBloqueada = true;
    guardarUsuario(usuario);
    cout << "Tarjeta bloqueada. Comuníquese con su banco para desbloquearla." << endl;
}

int main() {
    string numCuenta;
    string PIN;
    int opcion;

    cout << "=== Bienvenido al Cajero Automático ===" << endl;
    cout << "Ingrese su número de cuenta: ";
    cin >> numCuenta;

    Usuario usuario = identificarUsuario(numCuenta);

    if (usuario.numCuenta == numCuenta) {
        if (usuario.tarjetaBloqueada) {
            cout << "La tarjeta esta bloqueada. Comuníquese con su banco para desbloquearla." << endl;
            return 0;
        }

        int intentos = 0;
        bool PINCorrecto = false;
        while (intentos < 3 && !PINCorrecto) {
            cout << "Ingrese su PIN: ";
            cin >> PIN;
            PINCorrecto = verificarPIN(usuario, PIN);
            if (!PINCorrecto) {
                intentos++;
                if (intentos < 3) {
                    cout << "Le quedan " << 3 - intentos << " intento(s)." << endl;
                } else {
                    bloquearTarjeta(usuario);
                    return 0;
                }
            }
        }


        cout << "Bienvenido: " << usuario.nombreUsuario << "\n\n";
        cout << "=== Menú ===" << endl;
        cout << "1. Consultar saldoActual" << endl;
        cout << "2. Cambiar PIN de seguridad" << endl; 
        cout << "3. Depositar dinero" << endl;
        cout << "4. Retirar dinero" << endl;
        cout << "5. Transferir dinero" << endl;
        cout << "6. Salir" << endl;

        cout << "Ingrese la opción deseada: ";
        cin >> opcion;

        string numCuentaDestino;
        string pinCambiado;

       while (opcion != 6)
       {
      
        switch (opcion) {
            case 1:
                consultarsaldoActual(usuario);
                break;
            case 2: 
                int opcionCaseDos;
              

                cout << "¿Seguro desea cambiar su pin de seguridad? \n";
                cout << "introduzca 1 para cambiarlo o cualquier otro numero para CANCELAR \n";
                cin >> opcionCaseDos;
               
                if(opcionCaseDos == 1){
                     cout << "Ingrese el nuevo pin: ";
                     cin >> pinCambiado;
                     cambiarPin(usuario, pinCambiado);  
                } else {
                      cout << "Cancelando opcion... \n";
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
                cout << "Ingrese el número de cuenta destino: ";
                cin >> numCuentaDestino;
                cout << "Ingrese el monto a transferir: $";
                cin >> montoTransferencia;
                transferirDinero(usuario, numCuentaDestino, montoTransferencia);
                break;
            case 6:
                // Salir del programa
                cout << "Gracias por utilizar nuestro cajero automático. ¡Hasta luego!" << endl;
                return 0;
                break;
            default:
                cout << "Opción inválida. Por favor, seleccione una opción válida." << endl;
                break;
        }
         }
    } else {
        cout << "Número de cuenta inválido." << endl;
    }

    return 0;
}