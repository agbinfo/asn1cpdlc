#include <utility>
#include <memory>

namespace Asn1 {
  template<int Min_, int Max_, typename T>
  class Enumerated {
  public:
    Enumerated() : value_() {}
    Enumerated(T v) : value_(v) {}
    ~Enumerated() {}
    Enumerated(const Enumerated& v) : value_(v.value_) {}
    void operator=(T & v) {
      if (v < Min_ || v > Max_) return;
      value_ = v;
    }

  public:
    typedef T PrimitiveType;

  public:
    T value_;
  };

  class Choice {
  };

  class Sequence {
  };

  class Asn1NULL {};

  template<typename T>
  class Option {
  public:
    Option() {}
    typedef T type;
  };
}

class Mode : private Asn1::Enumerated<0, 1, int> { // min,max,type
public:
  Mode() : Enumerated() {}
  Mode(Enumerated::PrimitiveType value) /* implicit */ : Enumerated(value) {}
  ~Mode() {}
  Mode(const Mode& value) : Enumerated(value) {}
  Mode& operator=(Mode value) { Enumerated::operator=(value.value_); return *this; }

public:
  enum EnumValues {
    cpdlc = 0,
    dsc = 1
  };
};

// Construct from choice enumerator
//   DownlinkMessage msg = DownlinkMessage( DownlinkMessage::noMessage_idx ); // AsnNULL
//   DownlinkMessage msg = DownlinkMessage( DownlinkMessage::aTCDownlinkMessage_idx, PMCPDLCMessageSetVersion1::ATCDownlinkMessage );
// Copy construct
//   DownlinkMessage msg = DownlinkMessage( DownlinkMessage );
//
// DownlinkMessage::Option opt = msg.option();
// msg.opt<DownlinkMessage::noMessage_idx>();
// msg.noMessage()
//
class DownlinkMessage : public Asn1::Choice {
public:
  enum Option {
    noMessage_idx = 0,
    aTCDownlinkMessage_idx = 1
  } choice_;
public:
  void deleteOption() {
    if (option_) {
      switch (choice_) {
      case noMessage_idx:
        delete static_cast<Asn1::Option<Asn1::Asn1NULL>* >(option_);
        break;
      case aTCDownlinkMessage_idx:
        //delete static_cast<Asn1::Option<PMCPDLCMessageSetVersion1::ATCDownlinkMessage>* >(option_);
        break;
      }
      option_ = nullptr;
    }
  }
  const Asn1::Option<Asn1::Asn1NULL>& noMessage() {
    if (!option_ || noMessage_idx != choice_)
    {
      deleteOption();
      option_ = new Asn1::Option<Asn1::Asn1NULL>();
    }
    return *static_cast<Asn1::Option<Asn1::Asn1NULL>* >(option_);
  }
  // Asn1::Option<aTCDownlinkMessage_idx, PMCPDLCMessageSetVersion1::ATCDownlinkMessage>::type aTCDownlinkMessage;
  DownlinkMessage() : Choice() { }
  void swap(DownlinkMessage&) {
    //noMessage
  }
private:
  void *option_;
  //int choice_;
  // Asn1::Option<Asn1::Asn1NULL> 
};

template<typename T>
class Required
{
public:
  typedef T type;
};

template<typename T>
class Optional
{
public:
  typedef std::unique_ptr<T> type;
};

class StartDownMessage : public Asn1::Sequence {
public:
  StartDownMessage() {}
  ~StartDownMessage() {}
  StartDownMessage(const StartDownMessage&) : Sequence() {}
  StartDownMessage& operator=(StartDownMessage v_) { swap(v_); }

  void swap(StartDownMessage v_) {
    std::swap(mode, v_.mode);
    startDownlinkMessage.swap(v_.startDownlinkMessage);
  }

public:
  Required<Mode>::type mode;
  Required<DownlinkMessage>::type startDownlinkMessage;

};

namespace Inner {
  class DownlinkMessage2 {
  public:
    class NoMessage;
    class AtcDownlinkMessage;
    enum Option {
      noMessage_idx = 0,
      aTCDownlinkMessage_idx = 1
    } choice_;
  public:
    void deleteOption() {
      if (option_) {
        switch (choice_) {
        case noMessage_idx:
          delete static_cast<Asn1::Option<Asn1::Asn1NULL>*>(option_);
          break;
        case aTCDownlinkMessage_idx:
          //delete static_cast<Asn1::Option<PMCPDLCMessageSetVersion1::ATCDownlinkMessage>* >(option_);
          break;
        }
        option_ = nullptr;
      }
    }
    const Asn1::Option<Asn1::Asn1NULL>& noMessage() {
      if (!option_ || noMessage_idx != choice_)
      {
        deleteOption();
        option_ = new Asn1::Option<Asn1::Asn1NULL>();
      }
      return *static_cast<Asn1::Option<Asn1::Asn1NULL>*>(option_);
    }
    // Asn1::Option<aTCDownlinkMessage_idx, PMCPDLCMessageSetVersion1::ATCDownlinkMessage>::type aTCDownlinkMessage;
    DownlinkMessage2() { }
    void swap(DownlinkMessage2&) {
      //noMessage
    }
  private:
    void *option_;
    //int choice_;
    // Asn1::Option<Asn1::Asn1NULL> 
  };
}

template <int Choice>
class DownlinkMessage2 : public Inner::DownlinkMessage2
{

};

void main()
{

}