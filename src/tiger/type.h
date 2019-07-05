#ifndef TIGER_CC_TYPE_H
#define TIGER_CC_TYPE_H

class Type {
public:
    enum class Tag {
        RECORD,
        NIL,
        INT,
        STRING,
        ARRAY,
        NAME,
        VOID
    };

public:
    virtual ~Type() = default;

    virtual Tag GetType() {
        return Tag::VOID;
    }

    virtual bool IsRecordType() {
        return false;
    }

    virtual bool IsNilType() {
        return false;
    }

    virtual bool IsIntType() {
        return false;
    }

    virtual bool IsStringType() {
        return false;
    }

    virtual bool IsArrayType() {
        return false;
    }

    virtual bool IsNameType() {
        return false;
    }

    virtual bool IsVoidType() {
        return false;
    }
};

class RecordType: public Type {
public:
    bool IsRecordType() {
        return true;
    }
    
    Tag GetType() {
        return Tag::RECORD;
    }
};

class NilType: public Type {
public:
    bool IsNilType() {
        return true;
    }
    
    Tag GetType() {
        return Tag::NIL;
    }
};

class IntType: public Type {
public:
    bool IsIntType() {
        return true;
    }
    
    Tag GetType() {
        return Tag::INT;
    }
};

class StringType: public Type {
public:
    bool IsStringType() {
        return true;
    }
    
    Tag GetType() {
        return Tag::STRING;
    }
};

class ArrayType: public Type {
public:
    bool IsArrayType() {
        return true;
    }
    
    Tag GetType() {
        return Tag::ARRAY;
    }
};

class NameType: public Type {
public:
    bool IsNameType() {
        return true;
    }
    
    Tag GetType() {
        return Tag::NAME;
    }
};

class VoidType: public Type {
public:
    bool IsVoidType() {
        return true;
    }
    
    Tag GetType() {
        return Tag::VOID;
    }
};

#endif // TIGER_CC_TYPE_H