#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <locale>
#include <vector>
#include <map>
#include <iomanip>
#include <exception>
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
      virtual bool operator==(string to) const;
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
      virtual bool operator==(string to) const;
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
      virtual bool operator==(string to) const;
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
      virtual bool operator==(string to) const;
      friend ostream & operator<< (ostream & os, const MediaMessage & object);
};

class SmartCarrier {
   private:
      string carrier_name_;
      map <string,vector <Message *> > smart_phone_;
      //functions
      void Menu() const;
      int GetChoice();
      void ListAll ();
      void SearchMessage();
      void EraseMessage();
      void DisconnectAccount();
      void StopService();
      void AddMessageToAccount();
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
   SmartCarrier carrier = SmartCarrier("Acme Carrier");
   carrier.Init();
   carrier.StartService();
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

bool Message::operator==(string to) const {
   return to_ == to;
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

bool TextMessage::operator==(string to) const {
   return get_message_to() == to;
}

//overloaded << operator for Text Message class
ostream & operator << ( ostream & os, const TextMessage & object) {
   os << "From " << object.get_message_from() << "   |    To: " << object.get_message_to() << "     |    Message :"  << object.get_text_message() << endl;
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

bool VoiceMessage::operator==(string to) const {
   return get_message_to() == to;
}
//overloaded friend operator << 
ostream & operator << ( ostream & os, const VoiceMessage & object) {
   os << "From :" << object.get_message_from() << "   |   To: "  << object.get_message_to() << "     |    Length :"  << object.get_message_length() << endl;
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

bool MediaMessage::operator==(string to) const {
  return get_message_to() == to; 
}

//overloaded friend operator << 
ostream & operator << ( ostream & os, const MediaMessage & object) {
   os << "From : " << object.get_message_from() << "   |   To: " << object.get_message_to() << "     |  Size: "  << object.get_media_size() << endl;
   return os;
}


/********************************************************               SmartCarrier Class     *******************************************************      */
SmartCarrier::SmartCarrier() : carrier_name_("") {}
SmartCarrier::SmartCarrier(string name) : carrier_name_(name) {}

SmartCarrier::~SmartCarrier() {  //deallocates pointers in each vector
   map<string, vector<Message *> >::iterator m_iter;
   vector<Message *>::iterator v_iter;

   for (m_iter = smart_phone_.begin(); m_iter != smart_phone_.end();m_iter++) {
      for(v_iter = m_iter->second.begin(); v_iter != m_iter->second.end(); v_iter++ ) {
         delete *v_iter; //virtual descructor will be called to delete both derived and base class
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
      cout << phone_number << "phone " << endl; 
      Message * p_text_message = NULL;
      Message * p_voice_message = NULL;
      Message * p_media_message = NULL;
      
      for (int i = 0; i < texts; i++) {
         random_number = generate_number();
         cout << random_number << endl;
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
         p_media_message = new MediaMessage(phone_number,random_number,(i+1)*30);
         smart_phone_[phone_number].push_back(p_media_message);
      }
   }
   fin.close();
}

void SmartCarrier::StartService() {
   int choice;
   do {
      choice = GetChoice();
      switch (choice) {
         case 1:
            ListAll();
            break;
         case 2:
            SearchMessage();
            break;
         case 3:
            EraseMessage();
            break;
         case 4:
            DisconnectAccount();
            break;
         case 5:
            AddMessageToAccount();
            break;
         case 6:
            StopService();
            return;
         default:
            cout << " Please choose one of the 5 options above. " << endl;
            break;
      }
   } while ( choice != 5 );
}

void SmartCarrier::Menu() const {
   cout << setw(20) << "Acme Wireless" << endl;
   cout << setw(20) << "Message Utilization and Accoun Admin " << endl;
   
   cout << "1. List summary of message usages/charges for all accounts" << endl;
   cout << "2. Search a message from an account " << endl;
   cout << "3. Erase a message from an account " << endl;
   cout << "4. Disconnect an account " << endl;
   cout << "5. Add message to existing account " << endl;
   cout << "6. Quit " << endl;
   
}

int SmartCarrier::GetChoice() {
   Menu();
   cin.clear();
   int user_choice;
   cin >> user_choice;
   return user_choice;
}


void SmartCarrier::ListAll () {
   int total_messages,total_texts, total_voice,total_media;
   double total_charges;
   
   cout << "Phone number" << setw(10) << "Total messages" << setw(8) << "Text" << setw(9) << "Voice" << setw(9) <<  "Media" << setw(20) << "Total charges" << endl;
   
   map<string,vector <Message * > >::iterator m_iter;
   vector<Message *>::iterator v_iter;
   
   TextMessage * p_text_message = NULL;
   VoiceMessage * p_voice_message = NULL;
   MediaMessage * p_media_message = NULL;
   for (m_iter = smart_phone_.begin(); m_iter != smart_phone_.end(); m_iter++) {  
      total_charges = 0, total_texts = 0, total_voice = 0, total_media = 0;
      for (v_iter = m_iter->second.begin(); v_iter != m_iter->second.end(); v_iter++) {  //get data for the current phone number
         if ( (p_text_message = dynamic_cast<TextMessage *>(*v_iter)) != NULL ) {
            total_texts +=1;
            total_charges += total_texts * p_text_message->ComputeCharge();
         } else if ((p_voice_message = dynamic_cast<VoiceMessage *>(*v_iter)) != NULL )  {
            total_voice += 1;
            total_charges += total_voice * p_voice_message->ComputeCharge();
         } else if ( (p_media_message = dynamic_cast<MediaMessage *>(*v_iter)  ) != NULL  ) {
            total_media += 1;
            total_charges += total_media * p_media_message->ComputeCharge();
         }
      }
      //output the data for this particular number
      cout << m_iter->first << setw(10) << m_iter->second.size() << setw(10) << total_texts << setw(10) << total_voice << setw(10) << total_media << setw(20) << "$ " << total_charges << endl;
   }
   p_text_message = NULL;
   p_voice_message = NULL;
   p_media_message = NULL;  
}

//Searches for user inputted account, if found, displays information, else, error
void SmartCarrier::SearchMessage() {
   string account_number, sent_to, message_type;
   
   cout << "Enter account number ( in ###-###-#### format) " << endl;
   cin.ignore();
   cin.clear();
   getline(cin,account_number);
   
   cout << "Enter phone number message was sent to " << endl;
   cin.clear();
   getline(cin, sent_to);

   cout << "Enter message type (Text, Voice, Media)" << endl;
   cin.clear();
   getline(cin,message_type);

   map<string, vector<Message  *> > ::iterator m_iter;
   vector<Message *>::iterator v_iter;

   bool found = false;
   TextMessage * p_text_mes = NULL;
   VoiceMessage * p_voice_mes = NULL;
   MediaMessage * p_media_mes = NULL;
      try {
      map<string, vector<Message *> >::iterator m_iter;
      m_iter = smart_phone_.find(account_number);
      if (m_iter != smart_phone_.end()) {
         vector < Message *>::iterator v_iter;
         for (v_iter = m_iter->second.begin(); v_iter != m_iter->second.end(); v_iter++) {
            if (message_type == "text" && (( p_text_mes = dynamic_cast<TextMessage *>(*v_iter)) != NULL) && *p_text_mes == sent_to) {
               cout << *p_text_mes;
               found = true;
               break;
            } else if ( (message_type == "voice") && (p_voice_mes = dynamic_cast<VoiceMessage *>(*v_iter)) != NULL && (*p_voice_mes == sent_to)) {
               cout << *p_voice_mes;
               found = true;
               break;
            } else if ( (message_type == "media") && (p_media_mes = dynamic_cast<MediaMessage *>(*v_iter)) != NULL && (*p_media_mes == sent_to) ) {
               cout << *p_media_mes;
               found = true;
               break;
            }
         }
         if (!found) {
            throw "Message not found";
         }
      } else {
         throw "Account not found";
      }
      } catch(const char * msg) {
         cerr << msg << endl;
      }
      p_text_mes = NULL;
   p_voice_mes = NULL;
   p_media_mes = NULL; 
}

void SmartCarrier::EraseMessage() {
   string account_number, sent_to, message_type;

   cout << "Enter account number ( in ###-###-#### format) " << endl;
   cin.ignore();
   cin.clear();
   getline(cin,account_number);

   cout << "Enter phone number message was sent to " << endl;
   cin.clear();
   getline(cin, sent_to);

   cout << "Enter message type (text, voice, media)" << endl;
   cin.clear();
   getline(cin,message_type);
   
   bool found = false;
   TextMessage * p_text_mes = NULL;
   VoiceMessage * p_voice_mes = NULL;
   MediaMessage * p_media_mes = NULL;
   
   try {
      map<string, vector<Message *> >::iterator m_iter;
      m_iter = smart_phone_.find(account_number);
      if (m_iter != smart_phone_.end()) { 
         vector < Message *>::iterator v_iter;
         for (v_iter = m_iter->second.begin(); v_iter != m_iter->second.end(); v_iter++) {
            if (message_type == "text" && (( p_text_mes = dynamic_cast<TextMessage *>(*v_iter)) != NULL) && *p_text_mes == sent_to) {
               m_iter->second.erase(v_iter);
               delete p_text_mes;
               found = true;
               break;
            } else if ( (message_type == "voice") && (p_voice_mes = dynamic_cast<VoiceMessage *>(*v_iter)) != NULL && (*p_voice_mes == sent_to)) {
               m_iter->second.erase(v_iter);
               delete p_voice_mes;
               found = true;
               break;         
            } else if ( (message_type == "media") && (p_media_mes = dynamic_cast<MediaMessage *>(*v_iter)) != NULL && (*p_media_mes == sent_to) ) {
               m_iter->second.erase(v_iter);
               delete p_media_mes;
               found = true;
               break;
            }
         }
         if (!found) {
            throw "Message not found";
         }
         cout << "Deleted message " << endl;
      } else {
         throw "Account not found";
      }
   } catch(const char * msg) {
      cerr << msg << endl;
   }
   p_text_mes = NULL;
   p_voice_mes = NULL;
   p_media_mes = NULL;
}

void SmartCarrier::DisconnectAccount() {
   string number;
   cout << " Enter phone number " << endl;
   
   cin.ignore();
   cin.clear();
   getline(cin,number);
   
   bool found = false;
   
   map<string, vector<Message *> >::iterator m_iter;
   vector<Message *>::iterator v_iter;
   
   try {
   for (m_iter = smart_phone_.begin(); m_iter != smart_phone_.end(); m_iter++) { 
      if (number == m_iter->first) {
         for (v_iter = m_iter->second.begin(); v_iter != m_iter->second.end(); v_iter++) {
            delete *v_iter; //derived will also call base due to it being virtual:
         }
      smart_phone_.erase(m_iter->first);
      found = true;
      break; 
      }
   }
   if (!found) {
      throw "Account not found";
   } else {
      cout << "Deleted account " << number << endl;
   }
   } catch (const char * msg) {
      cerr << msg << endl;
   }
}

void SmartCarrier::StopService() {
   ListAll();
   map<string, vector<Message *> >::iterator m_iter;
   vector<Message *>::iterator v_iter;

   for (m_iter = smart_phone_.begin(); m_iter != smart_phone_.end();m_iter++) {
      for(v_iter = m_iter->second.begin(); v_iter != m_iter->second.end(); v_iter++ ) {
         delete *v_iter; //virtual descructor will be called to delete both derived and base class
      }
   }
   smart_phone_.clear();  
   cout << " Thanks for stopping by! Bye now!" << endl;
   exit(1); 
}

void SmartCarrier::AddMessageToAccount() {
   string phone, to, type;
   try {
      cout << " Select phone number you want to add a message to " << endl;
      cin.ignore();
      cin.clear();
      getline(cin,phone);
      
      map<string,vector<Message *> >::iterator m_iter;

      m_iter = smart_phone_.find(phone);
      if (m_iter == smart_phone_.end()) {
         throw "That number does not exist";
      }
   } catch (const char * msg) {
      cerr << msg << endl;
   }
   cout << " Enter the number you would like to send this message to " << endl;
   cin.ignore();
   cin.clear();
   getline(cin,to);

   cout << " Enter the type of message you want to add " << endl;
   cin.clear();
   getline(cin,type);
  
   Message * p_message = NULL;
  
   try { 
   if (type == "text" ) {
         string message; 
         cout << " Enter message " << endl;
         cin.ignore();
         cin.clear();
         getline(cin,message);
         p_message = new TextMessage(phone,to,message);
         smart_phone_[phone].push_back(p_message);
   } else if (type == "voice") {
         int length;
         cout << " enter length of message " << endl;
         cin.clear();
         cin >> length;
         p_message = new VoiceMessage(phone,to,length);
         smart_phone_[phone].push_back(p_message);
   } else if (type == "media") {
         int size;
         cout << " enter size of message " << endl;
         cin.clear();
         cin >> size;
         p_message = new MediaMessage(phone,to,size);
         smart_phone_[phone].push_back(p_message);
   } else {
      throw "Not a valid option";
   }
   } catch (const char * msg) {
      cerr << msg << endl;
   }
   cout << "Messag added to " << phone << endl;
   p_message = NULL;
   StartService();
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
