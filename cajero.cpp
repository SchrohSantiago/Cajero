#include <iostream> 
#include <fstream>  // Libreria para el manejo de archivos
#include <string>  // Libreria String para utilizar la funcion "getline" y "stod"
#include <iomanip> // Funcion para utilizar fixed

using namespace std;

struct Usuario   // Struct para dar la base del usuario
{
    string numCuenta;
    string PIN;
    double saldoActual;    
    bool tarjetaBloqueada;
    string nombreUsuario;
};

// Prototipos:

Usuario identificarUsuario(const string &numCuenta);    // Las constantes utilizadas en las funciones las implementamos porque son valores que tienen que conservarse.
void guardarUsuario(const Usuario &usuario);
bool verificarPIN(Usuario &usuario, const string &PIN);
void consultarSaldoActual(const Usuario &usuario);
void transferirDinero(Usuario &usuario, double monto);
void depositarDinero(Usuario &usuario, double monto);
void cambiarPin(Usuario &usuario, string &pinCambiado);
void retirarDinero(Usuario &usuario, double monto);
void bloquearTarjeta(Usuario &usuario);
void menuSeleccion(int &opcion);

int main()
{
    string numCuenta;
    string PIN;

    cout << "Bienvenido al Cajero Automatico" << endl
         << endl;
    cout << "Ingrese su numero de cuenta: " << endl;
    cin >> numCuenta;

    Usuario usuario = identificarUsuario(numCuenta); //Creamos una variable de tipo de dato Struct Usuario, la cual crea una copia del struct en la que identificamos al user y sus datos mediante "IdentificarUsario" el numCuenta.

    if (usuario.numCuenta == numCuenta)  //Si encuentra un usuario coincidente con el numero de cuenta que ingresa el usuario entra en el if
    {
        if (usuario.tarjetaBloqueada) // Veirificamos si el usuario posee la tarjeta bloqueada
        {
            cout << "La tarjeta esta bloqueada. Comuniquese con su banco para desbloquearla." << endl
                 << endl;
            return 0;
        }

        int intentos = 0;   // Contador para que el usuario tenga 3 intentos de poner el pin
        bool PINCorrecto = false;
        while (intentos < 3 && !PINCorrecto)  // que itere mientras intentos sea menor a 3 o el pin sea correcto
        {
            cout << "Ingrese su PIN: " << endl;
            cin >> PIN;
            PINCorrecto = verificarPIN(usuario, PIN);
            if (!PINCorrecto) // Si el pin no es correcto
            {
                intentos++; // Incremetamos el contador
                if (intentos < 3)
                {
                    cout << "Le quedan " << 3 - intentos << " intentos." << endl;
                }
                else // El contador "intentos" llega a 3 por ende entra en el else y bloquea la tarjeta
                {
                    bloquearTarjeta(usuario);
                    return 0;
                }
            }
        }

        string pinCambiado;
        int opcion;

        cout << endl;
        cout << "Bienvenido: " << usuario.nombreUsuario << endl;
        cout << endl;

        menuSeleccion(opcion);  // En una funcion aparte creamos el menu que visualiza el usuario mediante la consola

        while (opcion != 6)
        {
            switch (opcion)
            {
            case 1:
                consultarSaldoActual(usuario);
                break;
            case 2:
                int opcionCaseDos;
                cout << "¿Seguro desea cambiar su pin de seguridad?" << endl;  // El cambio de pin es algo importante para un Usuario, en el caso de que accidentalmente haya tocado la opcion 2 le preguntamos por su seguridad si realmente quiere cambiar su PIN
                cout << "Introduzca 1 para cambiarlo o cualquier otro numero para CANCELAR" << endl;
                cin >> opcionCaseDos;
                if (opcionCaseDos == 1)
                {
                    cout << "Ingrese el nuevo pin: ";
                    cin >> pinCambiado;
                    cambiarPin(usuario, pinCambiado);
                }
                else
                {
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
                cout << endl;
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

            int continua;   // Le preguntamos al usuario si quiere seguir usando el cajero
            cout << "Ingrese 1 si desea realizar una nueva operacion." << endl;
            cout << "Ingrese cualquier otro numero para finalizar." << endl;
            cin >> continua;

            if (continua == 1) // En el caso que ingresa 1 mostramos nuevamente el menu
            {
                menuSeleccion(opcion);
            }
            else
            {
                cout << "Muchas gracias por utilizar nuestros cajeros..." << endl;
                return 0;
            }
        }
    } else { // En el caso de que introduzca un numero de cuenta inexistente o invalido
    cout << "Numero de cuenta invalido" << endl;
}
return 0;
}

void menuSeleccion(int &opcion)
{
    cout << "••••••••••• MENU •••••••••••" << endl;
    cout << "1  Consultar Saldo" << endl;
    cout << "2  Cambiar PIN de seguridad" << endl;
    cout << "3  Depositar dinero" << endl;
    cout << "4  Retirar dinero" << endl;
    cout << "5  Transferir dinero" << endl;
    cout << "6  Salir" << endl;
    cout << "••••••••••••••••••••••••••••" << endl
         << endl;

    cout << "Ingrese la opcion deseada: ";
    cin >> opcion;
    cout << endl;
}

// Funcion para leer los datos de un usuario desde un archivo CSV
Usuario identificarUsuario(const string &numCuenta)   // Funcion para identificar usuario y todos sus demas datos, por esto mismo unicamente pasamos por constante numCuenta con su respectivo tipo de dato string por referencia para modificarlo de manera global y no local
{
    ifstream archivo("usuarios.csv");  // declaramos la variable archivo de tipo "ifstream" para adentrarnos en los datos del .CSV 
    Usuario usuario; // declaramos variable usuario con el tipo de dato Struct Usuario

    if (archivo.is_open()) // Si el archivo abre correctamente
    {
        string cuenta, pin, saldoActual, tarjetaBloqueada, nombreUsuario;  // Utilizamos cuenta en vez de numCuenta en el string para mostrar que es indiferente, lo que nos importa es el orden establecido por las comas.
        while (getline(archivo, cuenta, ',') && // Los getline son cruciales para que podamos extraer y leer correctamente los datos, si vemos en los argumentos de la funcion, el primer pertenece a la variable "archivo" en este caso al usuarios.csv, el segundo argumento es "cuenta", leera el primer dato de usuarios en este caso "numCuenta" y el tercer argumento tenemos una "," esto es para delimitar la lectura del getline y lo que explica porque los datos en los archivos .CSV se ordenan mediante comas. 
               getline(archivo, pin, ',') &&
               getline(archivo, saldoActual, ',') &&
               getline(archivo, tarjetaBloqueada, ',') &&
               getline(archivo, nombreUsuario))
        {
            if (cuenta == numCuenta) // Mediante este if podemos verificar si el numero de cuenta introducido por el usuario del cajero coincide con un numero de usuario existente en el CSV
            {
                usuario.numCuenta = numCuenta; // En el caso de que si lo encuentra copiamos todos los datos de ese numero de cuenta o usuario existente
                usuario.PIN = pin;  
                usuario.saldoActual = stod(saldoActual); // saldoActual es de tipo double, y el CSV unicamente se compone de strings, mediante "stod" casteamos a saldoActual
                usuario.tarjetaBloqueada = (tarjetaBloqueada == "1"); // Si la variable tarjetaBloqueada es 1 significa que es true, es decir que la tarjeta esta bloqueada.
                usuario.nombreUsuario = nombreUsuario;
                break;
            } // Ya tenemos creada nuestra copia del struct Usuario con los datos correspondiente.
        }
        archivo.close(); // Cerramos el archivo
    }
    return usuario;
}

// Funcion para guardar los datos de un usuario en un archivo CSV
void guardarUsuario(const Usuario &usuario)
{
    ifstream archivoEntrada("usuarios.csv"); //Abrimos el archivo de entrada para su lectura
    ofstream archivoSalida("temp.csv"); //Mediante la funcion ofstream abrimos para la ESCRITURA un archivo de salida temporal y para que hacemos esto? basicamente por seguridad, si en vez de tener un archivo temporal de escritura modificariamos el archivo actual podemos tener perdida de datos

    if (archivoEntrada.is_open() && archivoSalida.is_open()) // Si los dos archivos (salida/entrada) se abren correctamente
    {
        string cuenta, pin, saldoActual, tarjetaBloqueada, nombreUsuario; // Como hicimos anteriormente en la funcion "identificarUsuario" creamos las variables de tipo string para leer el csv
        while (getline(archivoEntrada, cuenta, ',') &&
               getline(archivoEntrada, pin, ',') &&
               getline(archivoEntrada, saldoActual, ',') &&
               getline(archivoEntrada, tarjetaBloqueada, ',') &&
               getline(archivoEntrada, nombreUsuario))
        {
            if (cuenta == usuario.numCuenta) // Si coincide el numero de cuenta
            { 
                archivoSalida << usuario.numCuenta << "," // Como si fuese un cout, escribimos en el archivo temporal los datos, respetando la estructura del CSV con la coma 
                              << usuario.PIN << ","
                              << usuario.saldoActual << ","
                              << (usuario.tarjetaBloqueada ? "1" : "0") << "," // Usamos un ternario para preguntar si la tarjeta del usuario esta bloqueada o no, en caso de que este bloqueada en el archivo temporal se escribira un 1, caso contrario (:) se escribe un 0
                              << usuario.nombreUsuario << endl;
            }
            else // Caso contrario que no coincide en el nuevo archivo temporal se escriben los datos como estaban originalmente
            {
                archivoSalida << cuenta << ","
                              << pin << ","
                              << saldoActual << ","
                              << tarjetaBloqueada << ","
                              << nombreUsuario << endl;
            }
        }
        archivoEntrada.close();
        archivoSalida.close(); // Cerramos los dos archivos y una vez cerrados borramos el .CSV original y lo templazamos por "temp.csv" donde van a estar los datos ya modificados y le realizamos un rename al nombre del .CSV original
        remove("usuarios.csv");
        rename("temp.csv", "usuarios.csv"); // la funcion rename si o si nos pide el cambio de nombre, para mantener el nombre original 
    }
}

bool verificarPIN(Usuario &usuario, const string &PIN) // Simple verificacion de PIN
{
    if (usuario.PIN == PIN) // Si el usuario identificado por su numero de cuenta tiene el mismo PIN que se ingresa al principio ejecuta el booleano en true 
    {
        return true;
    }
    else
    {
        cout << "PIN de seguridad incorrecto" << endl;
        return false;
    }
}

void consultarSaldoActual(const Usuario &usuario) //Simple funcion de saldo 
{
    cout << "Saldo actual: $" << fixed << setprecision(2) << usuario.saldoActual << endl; // Teniamos un error con el usuario de "Jorge" su numero era demaisado grande y por consola lo expresaba como "Saldo actual: $1.75346e+06" por ende investigando vimos que la libreria "iomanip"  tiene dos funciones especificas para solucionar esto y que por consola devuelva el numero decimal bien expresado, fixed y setprecision(), set precision segun el decimal que incluyas dentro de los parentesis es los 0 despues de las comas. 
}

void depositarDinero(Usuario &usuario, double monto)
{
    usuario.saldoActual += monto; 
    cout << "Se ha depositado $" << monto << " en la cuenta" << endl;
    cout << "Saldo actualizado: $" << usuario.saldoActual << endl;
    guardarUsuario(usuario); // Utilizamos la funcion guardarUsuario para modificar los datos del .CSV
}

void cambiarPin(Usuario &usuario, string &pinCambiado)
{
    if (pinCambiado.length() == 4) // Como es un string utilizamos ".length" para medir el largo, entonces si el largo del pin ingresado es igual a 4 ingresa en el if
    {
        usuario.PIN = pinCambiado;
        cout << "Su pin ha sido cambiado exitosamente" << endl;
        guardarUsuario(usuario);
    }
    else
    {
        cout << "Su pin debe tener unicamente 4 digitos" << endl;
    }
}

void retirarDinero(Usuario &usuario, double monto)
{
  if(monto <= 15000){
    if (monto <= usuario.saldoActual) // Verificamos que el monto a retirar por el usuario sea menor o igual a su saldo
    {
        usuario.saldoActual -= monto;
        cout << "Se ha retirado $" << monto << " de la cuenta" << endl;
        cout << "Saldo Actualizado: $" << usuario.saldoActual << endl;
        guardarUsuario(usuario); // Guardamos para actualizar el .csv
    }
    else
    {
        cout << "Saldo insuficiente" << endl;
    }
  } else {
       cout << "El monto maximo de retiro es de $15000" << endl << endl;
  }
}

void transferirDinero(Usuario &usuario, double monto)
{
    int confirmacion;
    string numCuentaDestino;
    bool finalizado = false;

    while(!finalizado){

    cout << "Ingrese el numero de cuenta destino: ";   // A diferencia de las demas funciones con la de transferir por motivos de lectura en cascada tuvimos que introducir la solicitud de la cuenta destino dentro de la misma funcion y no en el Switch del main
    cin >> numCuentaDestino;

        Usuario usuarioDestino = identificarUsuario(numCuentaDestino); // creamos una nueva variable que copie el struct, utilizando el "identificarUsuario" para que busque dentro del .CSV un numero de cuenta que coincida con el que introduce el usuario.

        if (usuarioDestino.numCuenta != "" && !usuarioDestino.tarjetaBloqueada && usuarioDestino.numCuenta != usuario.numCuenta) // Si el numero de cuenta existe, no esta bloqueado, y el usuario no introdujo su propio numero de cuenta ingresa en el if
        {
            cout << "Ingrese el monto a transferir: $"; 
            cin >> monto;

            if(monto <= 125000){ // Monto de transferencia maximo en los cajeros de bancos Argentinos

            cout << endl;
            cout << "¿Esta seguro que desea transferir la cantidad de $" << monto << " a " << usuarioDestino.nombreUsuario << "?" << endl << endl; // Ante una transferencia en un cajero normalmente deberia haber un aviso previo a transferir
            cout << "Ingrese 1 para CONFIRMAR o 2 para CANCELAR" << endl;
            cin >> confirmacion;
            cout << endl;

          

            if(confirmacion == 1){

            if(monto <= usuario.saldoActual){
            usuario.saldoActual -= monto; // Descontamos el dinero transferido del propio usuario
            usuarioDestino.saldoActual += monto; // Agregamos el dinero a la otra cuenta "usuarioDestino"
            cout << "Se ha transferido $" << monto << " a la cuenta " << numCuentaDestino << endl << endl;
            cout << "Saldo actual: $" << usuario.saldoActual << endl;
            guardarUsuario(usuario); // enviamos los datos modificados del usuario actual
            guardarUsuario(usuarioDestino); // enviamos los datos modificados del usuario que recibio el dinero 
            finalizado = true;
            } else {
                cout << "Error, monto insuficiente en su cuenta" << endl;     
                finalizado = true;
            }
            } else if(confirmacion == 2){
                finalizado = true;
            } else {
                cout << "Error en la seleccion de opcion" << endl << endl;
            }
          } else {
            cout << "El maximo de transferencia diario permitido es de $125000" << endl << endl;
        }
        }
        else
         {
            cout << "Error, Usuario inexistente o bloqueado." << endl;
            finalizado = true;
         }
      }  
}

void bloquearTarjeta(Usuario &usuario) //Simple funcion para bloquear la tarjeta
{
    usuario.tarjetaBloqueada = true;
    guardarUsuario(usuario);
    cout << "Tarjeta bloqueada, comuniquese al numero del banco 0800-232-1030 para desbloquearla." << endl
         << endl;
}