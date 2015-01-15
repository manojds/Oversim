//
// Generated file, do not edit! Created by opp_msgc 4.2 from common/ExtAPIMessages.msg.
//

#ifndef _EXTAPIMESSAGES_M_H_
#define _EXTAPIMESSAGES_M_H_

#include <omnetpp.h>

// opp_msgc version check
#define MSGC_VERSION 0x0402
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of opp_msgc: 'make clean' should help.
#endif

// cplusplus {{
#include <CommonMessages_m.h>

static const int GIAAPPCOMMAND_L = 8;
#define GIASEARCHAPP_L(msg) (GIAAPPCOMMAND_L)
#define GIAPUT_L(msg) (GIASEARCHAPP_L(msg) + msg->getKeysArraySize() * KEY_L)
#define GIAGET_L(msg) (GIASEARCHAPP_L(msg) + KEY_L + MAXRESPONSES_L)
#define GIAGETRSP_L(msg) (GIASEARCHAPP_L(msg) + KEY_L + NODEHANDLE_L)
// }}



/**
 * Enum generated from <tt>common/ExtAPIMessages.msg</tt> by opp_msgc.
 * <pre>
 * enum GIAAppCommand
 * {
 * 
 *     GIA_PUT = 0;
 *     GIA_SEARCH = 1;
 *     GIA_ANSWER = 2;
 * }
 * </pre>
 */
enum GIAAppCommand {
    GIA_PUT = 0,
    GIA_SEARCH = 1,
    GIA_ANSWER = 2
};

/**
 * Class generated from <tt>common/ExtAPIMessages.msg</tt> by opp_msgc.
 * <pre>
 * packet GIASearchAppMessage
 * {
 * 	int command enum (GIAAppCommand);
 * }
 * </pre>
 */
class GIASearchAppMessage : public ::cPacket
{
  protected:
    int command_var;

  private:
    void copy(const GIASearchAppMessage& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const GIASearchAppMessage&);

  public:
    GIASearchAppMessage(const char *name=NULL, int kind=0);
    GIASearchAppMessage(const GIASearchAppMessage& other);
    virtual ~GIASearchAppMessage();
    GIASearchAppMessage& operator=(const GIASearchAppMessage& other);
    virtual GIASearchAppMessage *dup() const {return new GIASearchAppMessage(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual int getCommand() const;
    virtual void setCommand(int command);
};

inline void doPacking(cCommBuffer *b, GIASearchAppMessage& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, GIASearchAppMessage& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>common/ExtAPIMessages.msg</tt> by opp_msgc.
 * <pre>
 * packet GIAput extends GIASearchAppMessage
 * {
 *     	OverlayKey keys[];
 * }
 * </pre>
 */
class GIAput : public ::GIASearchAppMessage
{
  protected:
    OverlayKey *keys_var; // array ptr
    unsigned int keys_arraysize;

  private:
    void copy(const GIAput& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const GIAput&);

  public:
    GIAput(const char *name=NULL, int kind=0);
    GIAput(const GIAput& other);
    virtual ~GIAput();
    GIAput& operator=(const GIAput& other);
    virtual GIAput *dup() const {return new GIAput(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual void setKeysArraySize(unsigned int size);
    virtual unsigned int getKeysArraySize() const;
    virtual OverlayKey& getKeys(unsigned int k);
    virtual const OverlayKey& getKeys(unsigned int k) const {return const_cast<GIAput*>(this)->getKeys(k);}
    virtual void setKeys(unsigned int k, const OverlayKey& keys);
};

inline void doPacking(cCommBuffer *b, GIAput& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, GIAput& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>common/ExtAPIMessages.msg</tt> by opp_msgc.
 * <pre>
 * packet GIAremove extends GIASearchAppMessage
 * {
 * }
 * </pre>
 */
class GIAremove : public ::GIASearchAppMessage
{
  protected:

  private:
    void copy(const GIAremove& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const GIAremove&);

  public:
    GIAremove(const char *name=NULL, int kind=0);
    GIAremove(const GIAremove& other);
    virtual ~GIAremove();
    GIAremove& operator=(const GIAremove& other);
    virtual GIAremove *dup() const {return new GIAremove(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
};

inline void doPacking(cCommBuffer *b, GIAremove& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, GIAremove& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>common/ExtAPIMessages.msg</tt> by opp_msgc.
 * <pre>
 * packet GIAsearch extends GIASearchAppMessage
 * {
 *    	OverlayKey searchKey;
 * 	int maxResponses;
 * }
 * </pre>
 */
class GIAsearch : public ::GIASearchAppMessage
{
  protected:
    OverlayKey searchKey_var;
    int maxResponses_var;

  private:
    void copy(const GIAsearch& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const GIAsearch&);

  public:
    GIAsearch(const char *name=NULL, int kind=0);
    GIAsearch(const GIAsearch& other);
    virtual ~GIAsearch();
    GIAsearch& operator=(const GIAsearch& other);
    virtual GIAsearch *dup() const {return new GIAsearch(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual OverlayKey& getSearchKey();
    virtual const OverlayKey& getSearchKey() const {return const_cast<GIAsearch*>(this)->getSearchKey();}
    virtual void setSearchKey(const OverlayKey& searchKey);
    virtual int getMaxResponses() const;
    virtual void setMaxResponses(int maxResponses);
};

inline void doPacking(cCommBuffer *b, GIAsearch& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, GIAsearch& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>common/ExtAPIMessages.msg</tt> by opp_msgc.
 * <pre>
 * packet GIAanswer extends GIASearchAppMessage
 * {
 *    	OverlayKey searchKey;
 * 	NodeHandle node;
 * }
 * </pre>
 */
class GIAanswer : public ::GIASearchAppMessage
{
  protected:
    OverlayKey searchKey_var;
    NodeHandle node_var;

  private:
    void copy(const GIAanswer& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const GIAanswer&);

  public:
    GIAanswer(const char *name=NULL, int kind=0);
    GIAanswer(const GIAanswer& other);
    virtual ~GIAanswer();
    GIAanswer& operator=(const GIAanswer& other);
    virtual GIAanswer *dup() const {return new GIAanswer(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual OverlayKey& getSearchKey();
    virtual const OverlayKey& getSearchKey() const {return const_cast<GIAanswer*>(this)->getSearchKey();}
    virtual void setSearchKey(const OverlayKey& searchKey);
    virtual NodeHandle& getNode();
    virtual const NodeHandle& getNode() const {return const_cast<GIAanswer*>(this)->getNode();}
    virtual void setNode(const NodeHandle& node);
};

inline void doPacking(cCommBuffer *b, GIAanswer& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, GIAanswer& obj) {obj.parsimUnpack(b);}


#endif // _EXTAPIMESSAGES_M_H_
