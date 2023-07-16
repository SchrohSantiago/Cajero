#include <iostream>
#include <fstream>
#include <string>


using namespace std;

struct Usuario {
    string numeroCuenta;
    string PIN;
    double saldo;
    bool bloqueada;
    string nombreUsuario;
};


// Función para leer los datos de un usuario desde un archivo CSV
Usuario leerUsuario(const string& numeroCuenta) {
    ifstream archivo("usuarios.csv");
    Usuario usuario;
    usuario.numeroCuenta = numeroCuenta;
    usuario.bloqueada = false;

    if (archivo.is_open()) {
        string cuenta, pin, saldo, bloqueada, nombreUsuario;
        while (getline(archivo, cuenta, ',') &&
               getline(archivo, pin, ',') &&
               getline(archivo, saldo, ',') &&
               getline(archivo, bloqueada, ',') &&
               getline(archivo, nombreUsuario)) {
            if (cuenta == numeroCuenta) {
                usuario.PIN = pin;
                usuario.saldo = stod(saldo);
                usuario.bloqueada = (bloqueada == "1");
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
        string cuenta, pin, saldo, bloqueada, nombreUsuario;
        while (getline(archivoEntrada, cuenta, ',') &&
               getline(archivoEntrada, pin, ',') &&
               getline(archivoEntrada, saldo, ',') &&
               getline(archivoEntrada, bloqueada, ',') &&
               getline(archivoEntrada, nombreUsuario)) {
            if (cuenta == usuario.numeroCuenta) {
                archivoSalida << usuario.numeroCuenta << ","
                              << usuario.PIN << ","
                              << usuario.saldo << ","
                              << (usuario.bloqueada ? "1" : "0") << ","
                              << usuario.nombreUsuario << endl;
            } else {
                archivoSalida << cuenta << ","
                              << pin << ","
                              << saldo << ","
                              << bloqueada << ","
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

// Función para consultar el saldo de la cuenta
void consultarSaldo(const Usuario& usuario) {
    cout << "Saldo actual: $" << usuario.saldo << endl;
}

// Función para depositar dinero en la cuenta
void depositarDinero(Usuario& usuario, double monto) {
    usuario.saldo += monto;
    cout << "Se ha depositado $" << monto << " en la cuenta." << endl;
    cout << "Saldo actual: $" << usuario.saldo << endl;
    guardarUsuario(usuario);
}

// Función para retirar dinero de la cuenta
void retirarDinero(Usuario& usuario, double monto) {
    if (monto <= usuario.saldo) {
        usuario.saldo -= monto;
        cout << "Se ha retirado $" << monto << " de la cuenta." << endl;
        cout << "Saldo actual: $" << usuario.saldo << endl;
        guardarUsuario(usuario);
    } else {
        cout << "Saldo insuficiente." << endl;
    }
}

// Función para transferir dinero a otra cuenta
void transferirDinero(Usuario& usuario, const string& numeroCuentaDestino, double monto) {
    if (monto <= usuario.saldo) {
        Usuario usuarioDestino = leerUsuario(numeroCuentaDestino);
        if (usuarioDestino.numeroCuenta == numeroCuentaDestino && !usuarioDestino.bloqueada) {
            usuario.saldo -= monto;
            usuarioDestino.saldo += monto;
            cout << "Se ha transferido $" << monto << " a la cuenta " << numeroCuentaDestino << "." << endl;
            cout << "Saldo actual: $" << usuario.saldo << endl;
            guardarUsuario(usuario);
            guardarUsuario(usuarioDestino);
        } else {
            cout << "La cuenta destino no existe o está bloqueada." << endl;
        }
    } else {
        cout << "Saldo insuficiente." << endl;
    }
}

// Función para bloquear la tarjeta del usuario
void bloquearTarjeta(Usuario& usuario) {
    usuario.bloqueada = true;
    guardarUsuario(usuario);
    cout << "Tarjeta bloqueada. Comuníquese con su banco para desbloquearla." << endl;
}

int main() {
    string numeroCuenta;
    string PIN;
    int opcion;

    cout << "=== Bienvenido al Cajero Automático ===" << endl;
    cout << "Ingrese su número de cuenta: ";
    cin >> numeroCuenta;

    Usuario usuario = leerUsuario(numeroCuenta);

    if (usuario.numeroCuenta == numeroCuenta) {
        if (usuario.bloqueada) {
            cout << "La tarjeta está bloqueada. Comuníquese con su banco para desbloquearla." << endl;
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
        cout << "1. Consultar saldo" << endl;
        cout << "2. Depositar dinero" << endl;
        cout << "3. Retirar dinero" << endl;
        cout << "4. Transferir dinero" << endl;
        cout << "5. Salir" << endl;

        cout << "Ingrese la opción deseada: ";
        cin >> opcion;

        string numeroCuentaDestino;

        switch (opcion) {
            case 1:
                consultarSaldo(usuario);
                break;
            case 2:
                double montoDeposito;
                cout << "Ingrese el monto a depositar: $";
                cin >> montoDeposito;
                depositarDinero(usuario, montoDeposito);
                break;
            case 3:
                double montoRetiro;
                cout << "Ingrese el monto a retirar: $";
                cin >> montoRetiro;
                retirarDinero(usuario, montoRetiro);
                break;
            case 4:
                
                double montoTransferencia;
                cout << "Ingrese el número de cuenta destino: ";
                cin >> numeroCuentaDestino;
                cout << "Ingrese el monto a transferir: $";
                cin >> montoTransferencia;
                transferirDinero(usuario, numeroCuentaDestino, montoTransferencia);
                break;
            case 5:
                // Salir del programa
                cout << "Gracias por utilizar nuestro cajero automático. ¡Hasta luego!" << endl;
                return 0;
                break;
            default:
                cout << "Opción inválida. Por favor, seleccione una opción válida." << endl;
                break;
        }
    } else {
        cout << "Número de cuenta inválido." << endl;
    }

    return 0;
}