#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <locale>
#include <vector>
#include <map>
#include <iomanip>

using namespace std;

//function used for generating random phone numbers
string generate_number();
string generate_string();  // generates a random text message(in alien talk)

//global index for string
int g_string_generator_index;

class Message {
   private:
      string from_; // senders phone number
      string to_; // recieves phone number
   public:
      Message();
      Message(string from, string to);
      virtual ~Message();

      string get_message_from() const;
      void set_message_from(string from);

      string get_message_to() const;
      void set_message_to(string to);

      virtual double ComputeCharge() = 0;
      virtual bool operator==(const Message & other) const;
};

class TextMessage : public Message {
   private:
      string text_message_; // test message
   public:
      TextMessage();
      TextMessage(string from,string to,string message);
      virtual ~TextMessage();

      string get_text_message() const;
      void set_text_message(string message);

      virtual double ComputeCharge();
      friend ostream & operator<< (ostream & os, const TextMessage & object); 
};

class VoiceMessage : public Message {
   private:
      int length_; //length of voice message
   public:
      VoiceMessage();
      VoiceMessage(string from,string to,int length);
      virtual ~VoiceMessage();
      
      int get_message_length() const;
      void set_message_length(int length);

      virtual double ComputeCharge();
      friend ostream & operator<< (ostream & os, const VoiceMessage & object);
};

class MediaMessage : public Message {
   private:
      int size_; //size in MB
   public:
      MediaMessage();
      MediaMessage(string from,string to,int size); //from and to is for Message constructor that will get called
      virtual ~MediaMessage();

      int get_media_size() const;
      void set_media_size(int size);

      virtual double ComputeCharge();
      friend ostream & operator<< (ostream & os, const MediaMessage & object);
};

class SmartCarrier {
   private:
      string carrier_name_;
      map <string,vector <Message *> > smart_phone_;
      //functions
      void Menu() const;
      int GetChoice();
      void ListAll () const;
      void SearchMessage();
      void EraseMessage();
      void DisconnectAccount();
      void StopService();

   public:
      SmartCarrier();
      SmartCarrier(string name);
      ~SmartCarrier();
      
      string get_carrier_name() const;
      void set_carrier_name(string name);

      void Init();
      void StartService();      
};

int main() {
   return 1;
}

/********************************************************                  ABSTRACT Message Class     *******************************************************      */     
Message::Message():from_(""),to_("") {}
Message::Message(string from, string to): from_(from), to_(to) {} 
Message::~Message() {}

double Message::ComputeCharge() {
   return 0.0;
}      

string Message::get_message_to()  const {
   return to_;
}

void Message::set_message_to(string to) {
   to_ = to;
}

string Message::get_message_from() const {
   return from_;
}

void Message::set_message_from(string from) {
   from_ = from;
}

bool Message::operator==( const Message & other) const {
   return (get_message_from() == other.get_message_from() && get_message_to() == other.get_message_to());
}

/********************************************************                  Text  Message Class     *******************************************************      */
TextMessage::TextMessage() : Message(),text_message_("") {}
TextMessage::TextMessage(string from,string to, string message) : Message(from,to), text_message_(message) {}
TextMessage::~TextMessage() {}

string TextMessage::get_text_message() const {
   return text_message_;
}

void TextMessage::set_text_message(string message) {
   text_message_ = message;
}

double TextMessage::ComputeCharge() {
   return .20;
}

//overloaded << operator for Text Message class
ostream & operator << ( ostream & os, const TextMessage & object) {
   os << object.get_text_message() << endl;
   return os;
}

/********************************************************                  Voice  Message Class     *******************************************************      */
VoiceMessage::VoiceMessage() : Message(), length_(0) {}
VoiceMessage::VoiceMessage(string from,string to, int length) : Message(from,to), length_(length) {}
VoiceMessage::~VoiceMessage() {}

int VoiceMessage::get_message_length() const {
   return length_;
}

void VoiceMessage::set_message_length(int length) {
   length_ = length;
}

double VoiceMessage::ComputeCharge() {
   return 1.5;
}

//overloaded friend operator << 
ostream & operator << ( ostream & os, const VoiceMessage & object) {
   os << object.get_message_length() << endl;
   return os;
}

/********************************************************               Media  Message Class     *******************************************************      */
MediaMessage::MediaMessage() : Message(), size_(0) {}
MediaMessage::MediaMessage(string from,string to,int size) : Message(from,to) , size_(size) {}
MediaMessage::~MediaMessage() {}

int MediaMessage::get_media_size()  const {
   return size_;
}

void MediaMessage::set_media_size(int size) {
   size_ = size;
}

double MediaMessage::ComputeCharge() {
   return .10;
}

//overloaded friend operator << 
ostream & operator << ( ostream & os, const MediaMessage & object) {
   os << object.get_media_size() << endl;
   return os;
}


/********************************************************               SmartCarrier Class     *******************************************************      */
SmartCarrier::SmartCarrier() : carrier_name_("") {}
SmartCarrier::SmartCarrier(string name) : carrier_name_(name) {}

SmartCarrier::~SmartCarrier() {  //deallocates pointers in each vector
   map<string,vector <Message * > >::iterator map_iter;
   vector<Message *>::iterator vec_iter;
   // map_iter->second points to the vector of Messages 
   for (map_iter = smart_phone_.begin(); map_iter != smart_phone_.end(); map_iter++) {
      for (vec_iter = map_iter->second.begin(); vec_iter != map_iter->second.end(); vec_iter++) {
         Message * p_message = *vec_iter;
         //delete from map
         smart_phone_.erase(map_iter->first);
         //now free Message object memory
         delete p_message;
      }
   }
   smart_phone_.clear();
}

string SmartCarrier::get_carrier_name() const {
   return carrier_name_;
}

void SmartCarrier::set_carrier_name(string name) {
   carrier_name_ = name;
}

void SmartCarrier::Init() {
   Message * p_text_message = NULL;
   Message * p_voice_message = NULL;
   Message * p_media_message = NULL;
   
   ifstream fin ("messages.txt");
   
   string phone_number,random_number,buffer,message;
   int texts,voice, media;
   if (!fin) {
      cout << " Error opening file \n";
      exit(-1);
   }
   
   while (getline (fin,buffer, ';')) {
      phone_number = buffer;
      
      getline(fin,buffer,';');
      texts = atoi(buffer.c_str());
      
      
      for (int i = 0; i < texts; i++) {
         random_number = generate_number();
         message = generate_string();
         p_text_message = new TextMessage(phone_number, random_number,message);
         smart_phone_[phone_number].push_back(p_text_message);
      }
      
      getline(fin,buffer,';');
      voice = atoi(buffer.c_str());

      for (int i = 0; i < voice; i++) {
         random_number = generate_number();
         p_voice_message = new VoiceMessage(phone_number,random_number,(i+1)*89);
         smart_phone_[phone_number].push_back(p_voice_message);
      }
      
      getline(fin,buffer,'\n');
      media = atoi(buffer.c_str());
      
      for (int i = 0; i < media;i++) {
         random_number = generate_number();
         p_media_message = new MediaMessage(phone_number,random_number,(i+4)*30);
         smart_phone_[phone_number].push_back(p_media_message);
      }
   }
   
   fin.close();
   delete p_media_message;
   delete p_voice_message;
   delete p_text_message;   
}

void SmartCarrier::StartService() {

}

void SmartCarrier::Menu() const {
   cout << setw(10) << "Acme Wireless" << endl;
   cout << setw(10) << "Message Utilization and Accoun Admin " << endl;
   
   cout << "1. List summary of message usages/charges for all accounts" << endl;
   cout << "2. Search a message from an account " << endl;
   cout << "3. Erase a message from an account " << endl;
   cout << "4. Disconnect an account " << endl;
   cout << "5. Quit " << endl;
   
}

int SmartCarrier::GetChoice() {
   cin.clear();
   cin.ignore();
   int user_choice;
   cin >> user_choice;
   return user_choice;
}


void SmartCarrier::ListAll () const {

}
void SmartCarrier::SearchMessage() {

}

void SmartCarrier::EraseMessage() {

}

void SmartCarrier::DisconnectAccount() {

}

void SmartCarrier::StopService() {

}


//helper functions
string generate_number() {
   ostringstream oss;
   oss << (rand() % 100 ) << '-' <<  (rand() % 10) << (rand() % 9 ) << (rand() % 8) << "-" << rand() % 9 << rand() % 8 << rand() % 7 << rand() % 6;
   return oss.str();
}

string generate_string() {
   const string phrases[] = { "HEY!", "HI!" , " How are you ? ", " Wanna go to Movies? " , "Do you like c++?", "Do you like JavaScript?" , "Do you like C#?", "I like to move it" };
   return phrases[g_string_generator_index++ % sizeof(phrases)];
}
