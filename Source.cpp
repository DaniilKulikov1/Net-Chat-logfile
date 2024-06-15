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

� � � � � � � � �___====-_ �_-====___
� � � � � �_--^^^#####// � � �\\#####^^^--_
� � � � _-^##########// ( � �) \\##########^-_
� � � �-############// �|\^^/| �\\############-
� � �_/############// � (@::@) � \\############\_
� � /#############(( � � \\// � � ))#############\
� �-###############\\ � �(oo) � �//###############-
� -#################\\ �/ VV \ �//#################-
�-###################\\/ � � �\//###################-
_#/|##########/\######( � /\ � )######/\##########|\#_
|/ |#/\#/\#/\/ �\#/\##\ �| �| �/##/\#/ �\/\#/\#/\#| \|
` �|/ �V �V �` � V �\#\| | �| |/#/ �V � ' �V �V �\| �'
� �` � ` �` � � �` � / | | �| | \ � ' � � �' �' � '
� � � � � � � � � � ( �| | �| | �)
� � � � � � � � � �__\ | | �| | /__
� � � � � � � � � (vvv(VVV)(VVV)vvv)

�)";
    cout << "----------------------------------------------------------------------------------------------------" << endl;
    cout << "���������� ��� ������������ �� ����� � ���!" << endl;
    cout << "��� ����� ���������:" << endl;
    cout << "������ ������� - ������� �����, ����������� ��������� ����, ������� �������, � 2 ������" << endl;
    cout << "������ ������ �������� �������������� �������� �������� ���������" << endl;
    cout << " " << endl;

    cout << "������������ ������� �������� ������:" << endl;
    cout << "������� ������� - �����, ����������� ������ ����, � ����� �����������" << endl;
    cout << "��������� ������������ �- ������������ ����� ������� � ������ � ������� ��� ���������������� ����" << endl;
    cout << "----------------------------------------------------------------------------------------------------" << endl;
}


class Logger
{


private:

    string Manual;
    ofstream Zeon; //������ �� ������� ������, ����� �������� ����������
    ofstream Vavilon;//������ �� ������� ������, ����� �������� ����������
    ifstream Dakota; //��� ��� �������� �����


public:

    std::shared_mutex mutex;


    Logger()
    {
        mutex.lock();

        Manual = "����������� ���� ������������. ��� ������� �������� ��������� - �����, ��� � �������� Logger - ������ ���� ��������� ����, � � Manual - ��� ��������. ����� ��������� ������ ����� ������� � ����� ��������� ���� Logger.txt � ��������. ����� �������� ���� Manual c ���� �� ���������.";
        ofstream Zeon("Manual.txt");
        if (Zeon.is_open())
        {
            Zeon << Manual << endl;
        }
        else
        {
            cout << "���� �� ������" << endl;
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
            cout << "���� �� ������" << endl;
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
            cout << "���� �� ��������" << endl;
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
    string recepient_p; /*����*/
    string send_p;/* ���*/
    string text_p; /*����� ���*/
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
        return recepient_p;//���
    };

    string getSend() const//����
    {
        return send_p;
    };

    string getText() const//���
    {
        return text_p;
    };
};

class User
{
private:
    string name_p; //���
    string password_p;//������

public:

    User()//
    {
        name_p = " ";
        password_p = " ";
    };

    //����������� �������� ����������
    User(const string& name, const string& password) :name_p(name), password_p(password)
    {};

    //�������
    string& getName()
    {
        return name_p;
    };

    string& getPassword()
    {
        return password_p;
    };
};

//������ ��������� 
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
            cout << "� ���� ���� ���������! ����: " << item.getRecepient() << ".�� ����: " << item.getSend() << endl;

        }
    
    }


};



class Chat
{
    Memory<Message>messages;
public:
    //�������� ���� ������
    void sendMessage(const string& recepient, const string& send, const string& text)
    {
        //������ ������ ������� ������ � ���������
        Message message1(recepient, send, text);

        messages.AddSms(message1);//�������� � ������
      //  Logger log(message1); ��� ������ - message �� ������, � ������, � ������
        Logger log("����: " + message1.getRecepient()+ ". " + message1.getText());
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


    //����� ����������� ���������� ������
    WSADATA wsaData;
    int iResult(5);

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        cout << "<���������� �� ����������������!" << endl;
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
        cout << "������ ������ �����������" << endl;
        WSACleanup();
        return 1;
    }

    SOCKET ConnectSocket = INVALID_SOCKET;
    ptr = result;
    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

    if (ConnectSocket == INVALID_SOCKET)
    {
        cout << "������ ������" << endl;
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
        cout<<"��� ���������� � ��������!"<<endl;
        WSACleanup();
        return 1;
    }
    //��� ��������� ����� ����������� ���������� winsock2


    vector<User>Tolpa;
    User* currentUser = nullptr;


    cout << "�����������\n";
    string username, password;
    cout << "������� �����: " << endl;
    cin >> username;
    cout << "������� ������: " << endl;
    cin >> password;
    User First(username, password);
    Tolpa.push_back(First);
    cout << "�� ������� ������������������!������ ������� � �������." << endl;

    cout << " \n";
    cout << "�������� ��������:\n ";

    cout << "1. ������� ���������\n";

    cout << "2. ����� � ������������ �������\n";

    cout << "3. ������� ����� �������\n";

    cout << "4. ����� �� ��������\n";

    cout << "5.�������� ��� � ��������� Manual �� �����\n";


    cout << " \n";

    cout << "� ���������� ����������� ����� ��������" << endl;
 
    int choice(9);

    Chat chat;



    do 
    {
        cin >> choice;
        switch (choice)
        {
            { case 1:
                if (currentUser != nullptr)
                { // ������ ���� ������������ �����
                    string recipient, text;
                    cout << "������� ����������: ";
                    cin >> recipient;
                    cout << "������� ����� ���������: ";
                    cin.ignore();
                    getline(cin, text);

                   
                    string mail_to_server = "����: " + recipient + " �� ����: " + currentUser->getName()  + "." + "���������: " + text;
                    const char* mail = mail_to_server.c_str();
                    int maillength = mail_to_server.length();
                    int bytesSent = send(ConnectSocket, mail, maillength, 0);

                    if (bytesSent == SOCKET_ERROR)
                    {
                        cout << "�� �����������. ������ ������" << endl;
                        break;
                    }
                    else
                    {
                        cout << "C�������� ������� ����������.\n";
                    }

                    char recvbuf[1024];
                    int iResult2 = recv(ConnectSocket, recvbuf, sizeof(recvbuf), 0);

                    string receivedData;

                    if (iResult2 > 0)
                    {
                        receivedData = string(recvbuf, iResult2);
                    } //���  � � ������� ����� ������� � ������ ������ � ����� ������� �� ��������� ������ � �������� � ������
                    if (iResult2 == 0)
                    {
                        cout << "������ ������� ��� - ��� ����������" << endl;
                    }
                    // ������� ����������, ���� ����� ������ ��, ��� �������� �� ������ �������!!!!!!!!!!!!!!!!!!!!!


                    size_t pos1 = receivedData.find("����: ") + strlen("����: ");
                    size_t pos2 = receivedData.find(" �� ����: ", pos1);
                    size_t pos3 = receivedData.find(" ���������: ", pos2) + strlen(" ���������: ");

                    string recipient1 = receivedData.substr(pos1, pos2 - pos1);
                    string sender1 = receivedData.substr(pos2 + strlen(" �� ����: "), pos3 - pos2 - strlen(" �� ����: "));
                    string message1 = receivedData.substr(pos3);

                    //������ ��� ����� ��������� � ������ ��������� ������, ����� ��������� ������ � ������� ����

                    chat.sendMessage(recipient1, sender1, message1);




                }
                else if (currentUser == nullptr)
                {
                    cout << "C������ ������� ����� 2. ��� ����������������� ����� 3, � ����� ������� ����� 2. \n";
                }
                    break;
       
            }

            {
            case 2:
                cout << "���� � ������������ �������\n" << endl;
                string username1, password1;
                cout << "������� �����: " << endl;
                cin >> username1;
                cout << "������� ������: " << endl;
                cin >> password1;

                for (int k = 0; k < Tolpa.size(); k++) {
                    User* user = &Tolpa[k]; // �������� ��������� �� ������������
                    if (user != nullptr && user->getName() == username1 && user->getPassword() == password1) {
                        currentUser = user; // ������������� currentUser ��������
                        cout << "�� ������� ����� � �������!" << endl;
                        chat.DisplayMessages();
                        break;
                    }
                }
                if (currentUser == nullptr) {
                    cout << "������������ ����� ��� ������." << endl;
                }
                break;
            }

            {
            case 3:
                cout << "�����������\n";
                cout << "������� �����: " << endl;
                cin >> username;
                cout << "������� ������: " << endl;
                cin >> password;
                User newUser(username, password);
                Tolpa.push_back(newUser);  //��� ����������� ���

                int lastuser = Tolpa.size() - 1;


                currentUser = &Tolpa.at(lastuser); //�������� � �������������� ���
                cout << "�� ������� ������������������!" << endl;
                cout << "  " << endl;
                //if (currentUser != nullptr) { // �������� �� nullptr
                //    chat.DisplayMessages();
                //}
                break;
            }

            {
            case 4:
                currentUser = nullptr; // ������������� currentUser � nullptr ��� ������
                cout << "����� ��������. ������� � ������������ ������� ����� 2 ��� ����������������� ����� 3\n";
                cout << " \n";
                cout << " \n";
                break;
            }

     

            {
            case 5:

                Manual.readLog();

                cout << endl;
                cout << "�� ������ ������ �� �����, ������ ��� ����� ������, ����� �� ����� ���������� ������ �����" << endl;
                cout << endl;
                cout << "�� ��������� ������� ����� �� ����\n";
                cout << "� ������� ��� ������ ����� �������� ��� �������\n";
                cout << "� ���� - ��� ������ ������� ������" << endl;
                break;
            }


        default:
            cout << "�� ����� ������������ ��������" << endl;
            break;


        }


    } while (choice != 5);

    return 0;
}
  