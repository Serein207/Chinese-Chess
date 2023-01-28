#ifndef PIECE_H
#define PIECE_H

#include <cmath>
#include <list>
#include <map>

#define RED_SIDE true
#define BLACK_SIDE false

using Pos = std::pair<int, int>;

class Piece {   //请不要继承QObject
public:
    mutable int x, y;
    //正数
    const enum class PieceType {
        NONE = 0,
        JIANG_SHUAI = 1,
        RED_SHUAI   = 1,    BLACK_JIANG     = -1,
        SHI = 2,
        RED_SHI     = 2,    BLACK_SHI       = -2,
        XIANG = 3,
        RED_XIANG   = 3,    BLACK_XIANG     = -3,
        MA = 4,
        RED_MA      = 4,    BLACK_MA        = -4,
        JU = 5,
        RED_JU      = 5,    BLACK_JU        = -5,
        PAO = 6,
        RED_PAO     = 6,    BLACK_PAO       = -6,
        BING_ZU = 7,
        RED_BING    = 7,    BLACK_ZU        = -7
    } type;
private:
    static const std::map<PieceType, wchar_t> dict;
public:
    static inline wchar_t convert(PieceType type) {
        return dict.at(type);
    }
    inline wchar_t text() const {
        return convert(type);
    }
    inline void move(int x, int y) const {  //请勿修改
        this->x = x;
        this->y = y;
    }
    // 棋子颜色 双方统一
    inline bool side() const {
        return type > PieceType::NONE;
    }
    inline const Pos pos() const {
        return std::make_pair(x, y);
    }
private:
    bool noThreat(int x, int y) const;
    virtual bool isBasicMove(int x, int y) const = 0;
public:
    virtual bool isValidMove(int x, int y) const {
        return isBasicMove(x, y) && noThreat(x, y);
    }
    // 我方不被将军时，可以吃掉对方的王，满足条件时对方被将军
    virtual bool isThreat(const std::list<std::pair<Pos, Piece::PieceType>>& pieces) const = 0;
    virtual const std::list<Pos> getPossibleMoves() const = 0;

    Piece(int x, int y, PieceType type) : x(x), y(y), type(type) {}
    virtual ~Piece() {} //虚析构
};

inline Piece::PieceType removeSide(Piece::PieceType type) {
    return (Piece::PieceType)std::abs((int)type);
}

inline bool getSide(Piece::PieceType type) {
    return type > Piece::PieceType::NONE;
}

class JiangShuai : public Piece {
public:
    JiangShuai(int x, int y, bool side)
        : Piece(x, y, side ? PieceType::RED_SHUAI : PieceType::BLACK_JIANG) {}
private:
    virtual bool isBasicMove(int x, int y) const;
public:
    virtual bool isThreat(const std::list<std::pair<Pos, Piece::PieceType>>& pieces) const;
    virtual const std::list<Pos> getPossibleMoves() const;
};

class Shi : public Piece {
public:
    Shi(int x, int y, bool side)
        : Piece(x, y, side ? PieceType::RED_SHI : PieceType::BLACK_SHI) {}
private:
    virtual bool isBasicMove(int x, int y) const;
public:
    virtual bool isThreat(const std::list<std::pair<Pos, Piece::PieceType>>& pieces) const;
    virtual const std::list<Pos> getPossibleMoves() const;
};

class Xiang : public Piece {
public:
    Xiang(int x, int y, bool side)
        : Piece(x, y, side ? PieceType::RED_XIANG : PieceType::BLACK_XIANG) {}
private:
    virtual bool isBasicMove(int x, int y) const;
public:
    virtual bool isThreat(const std::list<std::pair<Pos, Piece::PieceType>>& pieces) const;
    virtual const std::list<Pos> getPossibleMoves() const;
};

class Ma : public Piece {
public:
    Ma(int x, int y, bool side)
        : Piece(x, y, side ? PieceType::RED_MA : PieceType::BLACK_MA) {}
private:
    virtual bool isBasicMove(int x, int y) const;
public:
    virtual bool isThreat(const std::list<std::pair<Pos, Piece::PieceType>>& pieces) const;
    virtual const std::list<Pos> getPossibleMoves() const;
};

class Ju : public Piece {
public:
    Ju(int x, int y, bool side)
        : Piece(x, y, side ? PieceType::RED_JU : PieceType::BLACK_JU) {}
private:
    virtual bool isBasicMove(int x, int y) const;
public:
    virtual bool isThreat(const std::list<std::pair<Pos, Piece::PieceType>>& pieces) const;
    virtual const std::list<Pos> getPossibleMoves() const;
};

class Pao : public Piece {
public:
    Pao(int x, int y, bool side)
        : Piece(x, y, side ? PieceType::RED_PAO : PieceType::BLACK_PAO) {}
private:
    virtual bool isBasicMove(int x, int y) const;
public:
    virtual bool isThreat(const std::list<std::pair<Pos, Piece::PieceType>>& pieces) const;
    virtual const std::list<Pos> getPossibleMoves() const;
};

class BingZu : public Piece {
public:
    BingZu(int x, int y, bool side)
        : Piece(x, y, side ? PieceType::RED_BING : PieceType::BLACK_ZU) {}
private:
    virtual bool isBasicMove(int x, int y) const;
public:
    virtual bool isThreat(const std::list<std::pair<Pos, Piece::PieceType>>& pieces) const;
    virtual const std::list<Pos> getPossibleMoves() const;
};


#endif // PIECE_H
