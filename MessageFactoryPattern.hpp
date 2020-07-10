// class Message
// {
// public:
//     // Message specific pack method, to be defined according to the message's
//     // fields. The base class functions pack and unpack the common header
    
//     virtual int Pack(RawBuffer *pBuf) const
//     {
//         m_bitIndex = 0;
        
//         PackBits(pBuf, 10, m_hdr.value);
//         PackBits(pBuf, 6, m_hdr.version);
//         PackBits(pBuf, 16, m_hdr.source);
//         PackBits(pBuf, 16, m_hdr.destination);
        
//         return m_bitIndex;
//     }
   
//    // Corresponding unpack method.
//     virtual int Unpack(const RawBuffer *pBuf) const
//     {
//        m_bitIndex = 0;
//        m_hdr.value = UnpackBits(pBuf, 10);
//        m_hdr.version = UnpackBits(pBuf, 6 );
//        m_hdr.source = UnpackBits(pBuf, 16);
//        m_hdr.destination = UnpackBits(pBuf, 16);   
       
//        return m_bitIndex;
//     }
 
//     struct Header
//     { 
//        int type;
//        int version;
//        int source;
//        int destination;
//     };
    
// protected:
     
//     Header m_hdr;
    
//     // Bit index is used to keep track of the current packing/unpacking byte index.
//     int m_bitIndex;
    
//     // PackBits packs a specified number of bits at the next bit index. The method increments
//     // the bit index
//     void PackBits(RawBuffer *pBuf, int numBits, int value) const;
    
//     / UnpackBits packs a specified number of bits at the next bit index. The method increments
//     // the bit index
//     int UnpackBits(const RawBuffer *pBuf, int numBits) const;
    
    
// };

// // Type for message creation function pointers
// typedef Message *(*CreateMessageFunction)();

// class MessageFactory
// {
//     // Mapping of message type, version and function pointers. Note that different versions
//     // of the same message can be supported by this arragement. A different class is defined
//     // for each version
//     CreateMessageFunction m_messageCreateFunctions[MAX_MESSAGE_TYPES][MAX_MESSAGE_VERSIONS];

// public:

//    // Register Message saves the function pointer for a message object creation function
//    // for that class
   
//    void RegisterMessage(int messageType, int messageVersion, CreateMessageFunction function)
//    {
//        m_messageCreateFunctions[messageType][messageVersion] = function;
//    }
   
//    // Create Message invokes the message creation function specific to the message type and version
   
//    Message *CreateMessage(int messageType, int messageVersion)
//    {
//       CreateMessageFunction function = m_messageCreateFunctions[messageType][messageVersion];
//       Message *pMsg = (*function)();
      
//       return pMsg;
//    }   
// }

// class Processor
// {
// public:    
//     void ReceiveMessages()
//     {
//         RawBuffer *pBuf;
//         Message *pMsg;
        
//         while((pBuf = receive()))
//         {
//            pMsg = m_messageFactory.CreateMessage(pBuf->hdr.type, pBuf->hdr.version);
           
//            pMsg->Unpack(pBuf);
           
//            HandleMessage(pMsg);
           
//            delete pMsg;
//         }
//      }
     
//      void SendMessage(const Message *pMsg)
//      {
//          RawBuffer *pBuf;
         
//          pBuf = pMsg->Pack();
         
//          send(pMsg->Destination(), pBuf);
//      }
     
// protected:
//     MessageFactory m_messageFactory;
    
//      void HandleMessage (const Message *pMsg) = 0;
// };