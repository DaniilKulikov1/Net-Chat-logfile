#include<iostream>
#include<string>
#include<vector>
#include<winSock2.h>
#include<ws2tcpip.h>
#include<shared_mutex>
#include<fstream>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

void hello()
{

    cout << R"(

                 ___====-_  _-====___
           _--^^^#####//      \\#####^^^--_
        _-^##########// (    ) \\##########^-_
       -############//  |\^^/|  \\############-
     _/############//   (@::@)   \\############\_
    /#############((     \\//     ))#############\
   -###############\\    (oo)    //###############-
  -#################\\  / VV \  //#################-
 -###################\\/      \//###################-
_#/|##########/\######(   /\   )######/\##########|\#_
|/ |#/\#/\#/\/  \#/\##\  |  |  /##/\#/  \/\#/\#/\#| \|
`  |/  V  V  `   V  \#\| |  | |/#/  V   '  V  V  \|  '
   `   `  `      `   / | |  | | \   '      '  '   '
                    (  | |  | |  )
                   __\ | |  | | /__
                  (vvv(VVV)(VVV)vvv)

 )";
    cout << "----------------------------------------------------------------------------------------------------" << endl;
    cout << "ПРИВЕТСТУЮ ВАС ПОЛЬЗОВАТЕЛЬ ВЫ ВОШЛИ В ЧАТ!" << endl;
    cout << "Над чатом трудились:" << endl;
    cout << "Даниил Куликов - старший стажёр, разработчик механизма чата, классов месседж, и 2 мемори" << endl;
    cout << "ДАННАЯ ВЕРСИЯ СЕТЕВОГО ВЗАИМОДЕЙСТВИЯ НАПИСАНА Даниилом Куликовым" << endl;
    cout << " " << endl;

    cout << "Разработчики которые покинули проект:" << endl;
    cout << "Николай Леванов - стажёр, разработчик класса юзер, и блока регистрации" << endl;
    cout << "Владислав Колесниченко  -  разработчик аскей дизайна и работа с циклами для функционирования чата" << endl;
    cout << "----------------------------------------------------------------------------------------------------" << endl;
}


class Logger
{


private:

    string Manual;
    ofstream Zeon; //отсюда их убирать нельзя, чтобы сработал деструктор
    ofstream Vavilon;//отсюда их убирать нельзя, чтобы сработал деструктор
    ifstream Dakota; //тут для открытия файла


public:

    std::shared_mutex mutex;


    Logger()
    {
        mutex.lock();

        Manual = "Приветствую тебя пользователь. Это краткое описание блокнотов - помни, что в блокноте Logger - записи всех сообщений чата, а в Manual - это описание. Чтобы прочитать журнал логов найдите в папке текстовый файл Logger.txt и откройте. Также доступен файл Manual c этим же описанием.";
        ofstream Zeon("Manual.txt");
        if (Zeon.is_open())
        {
            Zeon << Manual << endl;
        }
        else
        {
            cout << "Файл не создан" << endl;
        }
        mutex.unlock();
    }

    Logger(const string& f)
    {
        mutex.lock();
        ofstream Vavilon("Logger.txt", ios::app);
        if (Vavilon.is_open())
        {
            Vavilon << f << endl;
        }
        else
        {
            cout << "Файл не создан" << endl;
        }
        mutex.unlock();
    }


    void readLog()
    {
        mutex.lock_shared();
        Dakota.open("Manual.txt");
        if (Dakota.is_open())
        {
            string line;
            while (getline(Dakota, line))
            {
                cout << line << endl;
            }
            Dakota.close();
        }
        else
        {
            cout << "Файл не открылся" << endl;
        }
        mutex.unlock_shared();
    }






    ~Logger()
    {
        if (Zeon.is_open())
        {
            Zeon.close();
        }
        if (Vavilon.is_open())
        {
            Vavilon.close();
        }

    }

};






class Message
{
private:
    string recepient_p; /*кому*/
    string send_p;/* кто*/
    string text_p; /*текст смс*/
public:

    Message()
    {
        recepient_p = " ";
        send_p = " ";
        text_p = " ";
    }

    Message(const string& recepient, const string& send, const string& text) :recepient_p(recepient), send_p(send), text_p(text)
    {};

    string getRecepient() const
    {
        return recepient_p;//кто
    };

    string getSend() const//кому
    {
        return send_p;
    };

    string getText() const//что
    {
        return text_p;
    };
};

class User
{
private:
    string name_p; //имя
    string password_p;//пароль

public:

    User()//
    {
        name_p = " ";
        password_p = " ";
    };

    //Конструктор быстрого заполнения
    User(const string& name, const string& password) :name_p(name), password_p(password)
    {};

    //геттеры
    string& getName()
    {
        return name_p;
    };

    string& getPassword()
    {
        return password_p;
    };
};

//массив сообщений 
template<typename T>
class Memory {
private:


    vector<T> data;
public:
   

    void AddSms(const T& value) 
    {
        data.push_back(value);
    }



    void Display()
    {
        for (const auto& item : data)
        {
            cout << "В чате есть сообщение! Кому: " << item.getRecepient() << ".От кого: " << item.getSend() << endl;

        }
    
    }


};



class Chat
{
    Memory<Message>messages;
public:
    //функцией берём данные
    void sendMessage(const string& recepient, const string& send, const string& text)
    {
        //создаём обьект другого класса и наполняем
        Message message1(recepient, send, text);

        messages.AddSms(message1);//помещаем в массив
      //  Logger log(message1); так нельзя - message не строка, а обьект, я ошибся
        Logger log("Кому: " + message1.getRecepient()+ ". " + message1.getText());
    };

    void DisplayMessages()
    {
        messages.Display();
    };
};





int main(int argc, char **argv)
{
    system("chcp 1251");
    hello();

    Logger Manual;


    //часть посвящённая библиотеке винсок
    WSADATA wsaData;
    int iResult(5);

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        cout << "<Библиотека не инициализирована!" << endl;
        return 1;
    }

    addrinfo *result = nullptr;
    addrinfo* ptr = nullptr;
    addrinfo hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    iResult = getaddrinfo(argv[1], "27015", &hints, &result);
    if (iResult != 0)
    {
        cout << "Взятие адреса провалилось" << endl;
        WSACleanup();
        return 1;
    }

    SOCKET ConnectSocket = INVALID_SOCKET;
    ptr = result;
    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

    if (ConnectSocket == INVALID_SOCKET)
    {
        cout << "Ошибка сокета" << endl;
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR)
    {
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
    }

    freeaddrinfo(result);
    if (ConnectSocket == INVALID_SOCKET) {
        cout<<"Нет соединения с сервером!"<<endl;
        WSACleanup();
        return 1;
    }
    //тут кончается часть посвящённая библиотеке winsock2


    vector<User>Tolpa;
    User* currentUser = nullptr;


    cout << "Регистрация\n";
    string username, password;
    cout << "Введите логин: " << endl;
    cin >> username;
    cout << "Введите пароль: " << endl;
    cin >> password;
    User First(username, password);
    Tolpa.push_back(First);
    cout << "Вы успешно зарегистрировались!Теперь войдите в аккаунт." << endl;

    cout << " \n";
    cout << "Выберите действие:\n ";

    cout << "1. Послать сообщение\n";

    cout << "2. Войти в существующий аккаунт\n";

    cout << "3. Создать новый аккаунт\n";

    cout << "4. Выход из аккаунта\n";

    cout << "5.Покинуть чат и прочитать Manual по логам\n";


    cout << " \n";

    cout << "В дальнейшем пользуйтесь этими кнопками" << endl;
 
    int choice(9);

    Chat chat;



    do 
    {
        cin >> choice;
        switch (choice)
        {
            { case 1:
                if (currentUser != nullptr)
                { // только если пользователь вошел
                    string recipient, text;
                    cout << "Введите получателя: ";
                    cin >> recipient;
                    cout << "Введите текст сообщения: ";
                    cin.ignore();
                    getline(cin, text);

                   
                    string mail_to_server = "кому: " + recipient + " от кого: " + currentUser->getName()  + "." + "Сообщение: " + text;
                    const char* mail = mail_to_server.c_str();
                    int maillength = mail_to_server.length();
                    int bytesSent = send(ConnectSocket, mail, maillength, 0);

                    if (bytesSent == SOCKET_ERROR)
                    {
                        cout << "Не отправилось. Ошибка сокета" << endl;
                        break;
                    }
                    else
                    {
                        cout << "Cообщение успешно отправлено.\n";
                    }

                    char recvbuf[1024];
                    int iResult2 = recv(ConnectSocket, recvbuf, sizeof(recvbuf), 0);

                    string receivedData;

                    if (iResult2 > 0)
                    {
                        receivedData = string(recvbuf, iResult2);
                    } //вот  я и записал ответ сервера в строку теперь её нужно разбить на отдельные строки и записать в мемори
                    if (iResult2 == 0)
                    {
                        cout << "Ответа сервера нет - нет соединения" << endl;
                    }
                    // сделаем переменные, куда будем писать то, что извлечём из строки ТУТТТТТ!!!!!!!!!!!!!!!!!!!!!


                    size_t pos1 = receivedData.find("кому: ") + strlen("кому: ");
                    size_t pos2 = receivedData.find(" от кого: ", pos1);
                    size_t pos3 = receivedData.find(" Сообщение: ", pos2) + strlen(" Сообщение: ");

                    string recipient1 = receivedData.substr(pos1, pos2 - pos1);
                    string sender1 = receivedData.substr(pos2 + strlen(" от кого: "), pos3 - pos2 - strlen(" от кого: "));
                    string message1 = receivedData.substr(pos3);

                    //Теперь это нужно сохранить в массив сообщений Мемори, вывод сообщений описан в случаях ниже

                    chat.sendMessage(recipient1, sender1, message1);




                }
                else if (currentUser == nullptr)
                {
                    cout << "Cначала войдите нажав 2. Или зарегестрируйтесь нажав 3, а потом пойдите нажав 2. \n";
                }
                    break;
       
            }

            {
            case 2:
                cout << "Вход в существующий аккаунт\n" << endl;
                string username1, password1;
                cout << "Введите логин: " << endl;
                cin >> username1;
                cout << "Введите пароль: " << endl;
                cin >> password1;

                for (int k = 0; k < Tolpa.size(); k++) {
                    User* user = &Tolpa[k]; // Получаем указатель на пользователя
                    if (user != nullptr && user->getName() == username1 && user->getPassword() == password1) {
                        currentUser = user; // Устанавливаем currentUser напрямую
                        cout << "Вы успешно вошли в аккаунт!" << endl;
                        chat.DisplayMessages();
                        break;
                    }
                }
                if (currentUser == nullptr) {
                    cout << "Неправильный логин или пароль." << endl;
                }
                break;
            }

            {
            case 3:
                cout << "Регистрация\n";
                cout << "Введите логин: " << endl;
                cin >> username;
                cout << "Введите пароль: " << endl;
                cin >> password;
                User newUser(username, password);
                Tolpa.push_back(newUser);  //тут использован стл

                int lastuser = Tolpa.size() - 1;


                currentUser = &Tolpa.at(lastuser); //изменено с использованием стл
                cout << "Вы успешно зарегистрировались!" << endl;
                cout << "  " << endl;
                //if (currentUser != nullptr) { // Проверка на nullptr
                //    chat.DisplayMessages();
                //}
                break;
            }

            {
            case 4:
                currentUser = nullptr; // Устанавливаем currentUser в nullptr при выходе
                cout << "Выход выполнен. Войдите в существующий аккаунт нажав 2 или зарегестрируйтесь нажав 3\n";
                cout << " \n";
                cout << " \n";
                break;
            }

     

            {
            case 5:

                Manual.readLog();

                cout << endl;
                cout << "Вы прочли мануал по логам, сейчас чат будет закрыт, чтобы вы могли посмотреть журнал логов" << endl;
                cout << endl;
                cout << "Вы выполнили учебный выход из чата\n";
                cout << "В будущем эта кнопка будет закрывть всю консоль\n";
                cout << "А пока - это только учебный проект" << endl;
                break;
            }


        default:
            cout << "Вы ввели неправильное значение" << endl;
            break;


        }


    } while (choice != 5);

    return 0;
}
  