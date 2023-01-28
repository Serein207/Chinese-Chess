#include "piece.h"
#include "board.h"
#include "algorithms.h"

using Type = Piece::PieceType;

// 字典
const std::map<Type, wchar_t> Piece::dict = {
    { Type::RED_SHUAI, L'帥' },
    { Type::BLACK_JIANG, L'將' },
    { Type::RED_SHI, L'仕' },
    { Type::BLACK_SHI, L'士' },
    { Type::RED_XIANG, L'相' },
    { Type::BLACK_XIANG, L'象' },
    { Type::RED_MA, L'馬' },
    { Type::BLACK_MA, L'馬' },
    { Type::RED_JU, L'車' },
    { Type::BLACK_JU, L'車' },
    { Type::RED_PAO, L'炮' },
    { Type::BLACK_PAO, L'炮' },
    { Type::RED_BING, L'兵' },
    { Type::BLACK_ZU, L'卒' }
};

// 如果对方将军 我方不可以将军对方
// 解将成功
bool Piece::noThreat(int x, int y) const {
    auto pieces = Board::getBoard()->find();
    for (auto& [pos, type] : pieces) {
        if (pos == this->pos()) {
            pos = std::make_pair(x, y);
            type = this->type;
        }
    }
    if (Algorithms::isCheck(pieces, this->side()))
        return false;
    return true;
}

////////////////////////将帅

bool JiangShuai::isBasicMove(int x, int y) const {
    if(this->side() == Board::getBoard()->side()) {
        if (4 <= x && x <= 6 && 1 <= y && y <= 3) {
            if (std::abs(x - this->x) == 1 && this->y == y) {
                return true;
            } else if (std::abs(y - this->y) == 1 && this->x == x) {
                return true;
            }
        } else if ((this->x == x)
                   && (Board::getBoard()->find(x, -1, -1).size() == 2)
                   && removeSide(Board::getBoard()->find(x, -1, !side()).begin()->second) == PieceType::JIANG_SHUAI) {
            return true;
        }
    } else {
        if (4 <= x && x <= 6 && 8 <= y && y <= 10) {
            if (std::abs(x - this->x) == 1 && this->y == y) {
                return true;
            } else if (std::abs(y - this->y) == 1 && this->x == x) {
                return true;
            }
        } else if ((this->x == x)
                   && (Board::getBoard()->find(x, -1, side()).size() == 2)
                   && removeSide(Board::getBoard()->find(x, -1, !side()).begin()->second) == PieceType::JIANG_SHUAI) {
            return true;
        }
    }

    return false;
}

bool JiangShuai::isThreat(const std::list<std::pair<Pos, Piece::PieceType>>& pieces) const {
    if (Board::getBoard()->isChecked(side()))
        return false;

    auto find = [=](int x = -1, int y = -1, int side = -1) {
        std::list<std::pair<Pos, Piece::PieceType>> list;
        for (const auto& [pos, type] : pieces) {
            if (pos.first == ((x == -1) ? pos.first : x)) {
                if (pos.second == ((y == -1) ? pos.second : y)) {
                    if (getSide(type) == ((side == -1) ? getSide(type) : side)) {
                        list.emplace_back(std::make_pair(pos, type));
                    }
                }
            }
        }
        return list;
    };

    Pos jiangShuaiPos, myself;
    for (const auto& [pos, type] : pieces) {
        if (getSide(this->type) != getSide(type)             // 棋子不属于我方
                && removeSide(type) == PieceType::JIANG_SHUAI)      // 棋子是对方的将/帅
            jiangShuaiPos = pos;
    }
    for (const auto& [pos, type] : pieces) {
        if (type == this->type) {
            myself = pos;

            if ((myself.first == jiangShuaiPos.first)
                   && (find(myself.first, -1, -1).size() == 2)
                   && (removeSide(find(myself.first, -1, !side()).begin()->second) == PieceType::JIANG_SHUAI))
            return true;
        }
    }

    return false;
}

const std::list<Pos> JiangShuai::getPossibleMoves() const {
    std::list<Pos> positions{};
    for (int x = 1; x <= 9; x++) {
        for (int y = 1; y <= 10; y++) {
            if (this->isValidMove(x, y) && Board::getBoard()->find(x, y, side()).empty()) {
                positions.emplace_back(std::make_pair(x, y));
            }
        }
    }
    return positions;
}

////////////////////////士

bool Shi::isBasicMove(int x, int y) const {
    if (this->side() == Board::getBoard()->side()) {
        if (4 <= x && x <= 6 && 1 <= y && y <= 3) {
            if (std::abs(x - this->x) == 1 && std::abs(y - this->y) == 1) {
                return true;
            }
        }
    } else {
        if (4 <= x && x <= 6 && 8 <= y && y <= 10) {
            if (std::abs(x - this->x) == 1 && std::abs(y - this->y) == 1) {
                return true;
            }
        }
    }

    return false;
}

bool Shi::isThreat(const std::list<std::pair<Pos, Piece::PieceType>>& pieces) const {
    return false;
}

const std::list<Pos> Shi::getPossibleMoves() const {
    std::list<Pos> pos{};
    for (int x = 1; x <= 9; x++) {
        for (int y = 1; y <= 10; y++) {
            if (this->isValidMove(x, y) && Board::getBoard()->find(x, y, side()).empty()) {
                pos.emplace_back(std::make_pair(x, y));
            }
        }
    }
    return pos;
}

////////////////////////象

bool Xiang::isBasicMove(int x, int y) const {
    auto judge = [=](int sub1, int sub2)->bool {
        if (x - this->x == sub1 && y - this->y == sub2) {
            if (Board::getBoard()->find(this->x + sub1/2, this->y + sub2/2, -1).empty())
                return true;
        }
        return false;
    };

    if (this->side() == Board::getBoard()->side()) {
        if(y > 5)
            return false;
        if(judge(2, 2) || judge(2, -2) || judge(-2, 2) || judge(-2, -2))
            return true;
    } else {
        if(y < 6)
            return false;
        if(judge(2, 2) || judge(2, -2) || judge(-2, 2) || judge(-2, -2))
            return true;
    }

    return false;
}

bool Xiang::isThreat(const std::list<std::pair<Pos, Piece::PieceType>>& pieces) const {
    return false;
}

const std::list<Pos> Xiang::getPossibleMoves() const {
    std::list<Pos> pos{};
    for (int x = 1; x <= 9; x++) {
        for (int y = 1; y <= 10; y++) {
            if (this->isValidMove(x, y) && Board::getBoard()->find(x, y, side()).empty()) {
                pos.emplace_back(std::make_pair(x, y));
            }
        }
    }
    return pos;
}

////////////////////////马

bool Ma::isBasicMove(int x, int y) const {
    auto judge = [=](int sub1, int sub2)->bool {
        if (x - this->x == sub1 && y - this->y == sub2) {
            if (Board::getBoard()->find(this->x + sub1/2, this->y + sub2/2, -1).empty())
                return true;
        }
        return false;
    };
    if(judge(1, 2) || judge(1, -2) || judge(-1, 2) || judge(-1, -2)
        || judge(2, 1) || judge(-2, 1) || judge(2, -1) || judge(-2, -1))
        return true;
    return false;
}

bool Ma::isThreat(const std::list<std::pair<Pos, Piece::PieceType>>& pieces) const {
    if (Board::getBoard()->isChecked(side()))
        return false;

    auto find = [=](int x = -1, int y = -1, int side = -1) {
        std::list<std::pair<Pos, Piece::PieceType>> list;
        for (const auto& [pos, type] : pieces) {
            if (pos.first == ((x == -1) ? pos.first : x)) {
                if (pos.second == ((y == -1) ? pos.second : y)) {
                    if (getSide(type) == ((side == -1) ? getSide(type) : side)) {
                        list.emplace_back(std::make_pair(pos, type));
                    }
                }
            }
        }
        return list;
    };

    Pos jiangShuaiPos, myself;
    for (const auto& [pos, type] : pieces) {
        if (getSide(this->type) != getSide(type)             // 棋子不属于我方
                && removeSide(type) == PieceType::JIANG_SHUAI)      // 棋子是对方的将/帅
            jiangShuaiPos = pos;
    }
    for (const auto& [pos, type] : pieces) {
        if (type == this->type) {
            myself = pos;

            auto judge = [=](int sub1, int sub2)->bool {
                if (jiangShuaiPos.first - myself.first == sub1 && jiangShuaiPos.second - myself.second == sub2) {
                    if (Board::getBoard()->find(myself.first + sub1/2, myself.second + sub2/2, -1).empty())
                    return true;
                }
                return false;
            };

            if(judge(1, 2) || judge(1, -2) || judge(-1, 2) || judge(-1, -2)
                || judge(2, 1) || judge(-2, 1) || judge(2, -1) || judge(-2, -1))
                return true;
        }
    }

    return false;
}

const std::list<Pos> Ma::getPossibleMoves() const {
    std::list<Pos> pos{};
    for (int x = 1; x <= 9; x++) {
        for (int y = 1; y <= 10; y++) {
            if (this->isValidMove(x, y) && Board::getBoard()->find(x, y, side()).empty()) {
                pos.emplace_back(std::make_pair(x, y));
            }
        }
    }
    return pos;
}

////////////////////////车

bool Ju::isBasicMove(int x, int y) const {
    if(x == this->x) {  // 纵向
        const auto& pieces = Board::getBoard()->find(x, -1, -1);

        int upNearest = 11;
        int downNearest = 0;
        for (const auto& [pos, type] : pieces) {
            if (pos.second <= upNearest && pos.second > this->y) {
                upNearest = pos.second;
                if((int)type * (int)this->type > 0) // pos位置的棋子和此棋子是同一方
                    --upNearest;
            }
            if (pos.second >= downNearest && pos.second < this->y) {
                downNearest = pos.second;
                if ((int)type * (int)this->type > 0)
                    ++downNearest;
            }
        }

        if (downNearest <= y && y <= upNearest)
            return true;
    } else if (y == this->y) {  // 横向
        const auto& pieces = Board::getBoard()->find(-1, y, -1);

        int leftNearest = 0;
        int rightNearest = 10;
        for (const auto& [pos, type] : pieces) {
            if(pos.first >= leftNearest && pos.first < this->x) {
                leftNearest = pos.first;
                if((int)type * (int)this->type > 0)
                    ++leftNearest;
            }
            if(pos.first <= rightNearest && pos.first > this->x) {
                rightNearest = pos.first;
                if((int)type * (int)this->type > 0)
                    --rightNearest;
            }
        }

        if (leftNearest <= x && x <= rightNearest)
            return true;
    }
    return false;
}

bool Ju::isThreat(const std::list<std::pair<Pos, Piece::PieceType>>& pieces) const {
    if (Board::getBoard()->isChecked(side()))
        return false;

    auto find = [=](int x = -1, int y = -1, int side = -1) {
        std::list<std::pair<Pos, Piece::PieceType>> list;
        for (const auto& [pos, type] : pieces) {
            if (pos.first == ((x == -1) ? pos.first : x)) {
                if (pos.second == ((y == -1) ? pos.second : y)) {
                    if (getSide(type) == ((side == -1) ? getSide(type) : side)) {
                        list.emplace_back(std::make_pair(pos, type));
                    }
                }
            }
        }
        return list;
    };

    Pos jiangShuaiPos, myself;
    for (const auto& [pos, type] : pieces) {
        if (Board::getBoard()->side() != getSide(type)              // 棋子不属于我方
                && removeSide(type) == PieceType::JIANG_SHUAI)      // 棋子是对方的将/帅
            jiangShuaiPos = pos;
    }
    for (const auto& [pos, type] : pieces) {
        if (type == this->type) {
            myself = pos;
            if(myself.first == jiangShuaiPos.first) {  // 纵向
                const auto& pieces = find(myself.first, -1, -1);

                int upNearest = 11;
                int downNearest = 0;
                for (const auto& [pos, type] : pieces) {
                    if (pos.second <= upNearest && pos.second > myself.second) {
                        upNearest = pos.second;
                        if((int)type * (int)this->type > 0) // pos位置的棋子和此棋子是同一方
                            --upNearest;
                    }
                    if (pos.second >= downNearest && pos.second < myself.second) {
                        downNearest = pos.second;
                        if ((int)type * (int)this->type > 0)
                            ++downNearest;
                    }
                }

                if (downNearest <= jiangShuaiPos.second && jiangShuaiPos.second <= upNearest)
                    return true;
            } else if (myself.second == jiangShuaiPos.second) {  // 横向
                const auto& pieces = find(-1, myself.second, -1);

                int leftNearest = 0;
                int rightNearest = 10;
                for (const auto& [pos, type] : pieces) {
                    if(pos.first >= leftNearest && pos.first < myself.first) {
                        leftNearest = pos.first;
                        if((int)type * (int)this->type > 0)
                            ++leftNearest;
                    }
                    if(pos.first <= rightNearest && pos.first > myself.first) {
                        rightNearest = pos.first;
                        if((int)type * (int)this->type > 0)
                            --rightNearest;
                    }
                }

                if (leftNearest <= jiangShuaiPos.first && jiangShuaiPos.first <= rightNearest)
                    return true;
            }
        }
    }

    return false;
}

const std::list<Pos> Ju::getPossibleMoves() const {
    std::list<Pos> pos{};
    for (int x = 1; x <= 9; x++) {
        for (int y = 1; y <= 10; y++) {
            if (this->isValidMove(x, y) && Board::getBoard()->find(x, y, side()).empty()) {
                pos.emplace_back(std::make_pair(x, y));
            }
        }
    }
    return pos;
}

////////////////////////炮

bool Pao::isBasicMove(int x, int y) const {   
    if(x == this->x) {  // 纵向
        const auto& pieces = Board::getBoard()->find(x, -1, -1);

        int upNearest = 11, upSecNearest = 11, upEnemy = 11;
        int downNearest = 0, downSecNearest = 0, downEnemy = 0;
        for (const auto& [pos, type] : pieces) {
            if (pos.second <= upNearest && pos.second > this->y) {
                upNearest = pos.second;
            }
            if (pos.second >= downNearest && pos.second < this->y) {
                downNearest = pos.second;
            }
        }
        for (const auto& [pos, type] : pieces) {
            if (pos.second <= upSecNearest && pos.second > upNearest) {
                upSecNearest = pos.second;
            }
            if (pos.second >= downSecNearest && pos.second < downNearest) {
                downSecNearest = pos.second;
            }
        }
        for (const auto& [pos, type] : pieces) {
            if (pos.second == upSecNearest && (int)type * (int)this->type < 0) {
                upEnemy = pos.second;
            }
            if (pos.second == downSecNearest && (int)type * (int)this->type < 0) {
                downEnemy = pos.second;
            }
        }

        if (downNearest < y && y < upNearest)
            return true;
        if (y == upEnemy || y == downEnemy)
            return true;
    }
    else if (y == this->y) {  // 横向
        const auto& pieces = Board::getBoard()->find(-1, y, -1);

        int rightNearest = 10, rightSecNearest = 10, rightEnemy = 10;
        int leftNearest = 0, leftSecNearest = 0, leftEnemy = 0;
        for (const auto& [pos, type] : pieces) {
            if (pos.first <= rightNearest && pos.first > this->x) {
                rightNearest = pos.first;
            }
            if (pos.first >= leftNearest && pos.first < this->x) {
                leftNearest = pos.first;
            }
        }
        for (const auto& [pos, type] : pieces) {
            if (pos.first <= rightSecNearest && pos.first > rightNearest) {
                rightSecNearest = pos.first;
            }
            if (pos.first >= leftSecNearest && pos.first < leftNearest) {
                leftSecNearest = pos.first;
            }
        }
        for (const auto& [pos, type] : pieces) {
            if (pos.second == rightSecNearest && (int)type * (int)this->type < 0) {
                rightEnemy = pos.second;
            }
            if (pos.second == leftSecNearest && (int)type * (int)this->type < 0) {
                leftEnemy = pos.second;
            }
        }

        if (leftNearest < x && x < rightNearest)
            return true;
        if (x == leftEnemy || x == rightEnemy)
            return true;
    }
    return false;
}

bool Pao::isThreat(const std::list<std::pair<Pos, Piece::PieceType>>& pieces) const {
    if (Board::getBoard()->isChecked(side()))
        return false;

    auto find = [=](int x = -1, int y = -1, int side = -1) {
        std::list<std::pair<Pos, Piece::PieceType>> list;
        for (const auto& [pos, type] : pieces) {
            if (pos.first == ((x == -1) ? pos.first : x)) {
                if (pos.second == ((y == -1) ? pos.second : y)) {
                    if (getSide(type) == ((side == -1) ? getSide(type) : side)) {
                        list.emplace_back(std::make_pair(pos, type));
                    }
                }
            }
        }
        return list;
    };

    Pos jiangShuaiPos, myself;
    for (const auto& [pos, type] : pieces) {
        if (getSide(this->type) != getSide(type)              // 棋子不属于我方
                && removeSide(type) == PieceType::JIANG_SHUAI)      // 棋子是对方的将/帅
            jiangShuaiPos = pos;
    }
    for (const auto& [pos, type] : pieces) {
        if (type == this->type) {
            myself = pos;
            if (myself.first == jiangShuaiPos.first) {  // 纵向
                const auto& pieces = find(myself.first, -1, -1);

                int upNearest = 11, upSecNearest = 11, upEnemy = 11;
                int downNearest = 0, downSecNearest = 0, downEnemy = 0;
                for (const auto& [pos, type] : pieces) {
                    if (pos.second <= upNearest && pos.second > myself.second) {
                        upNearest = pos.second;
                    }
                    if (pos.second >= downNearest && pos.second < myself.second) {
                        downNearest = pos.second;
                    }
                }
                for (const auto& [pos, type] : pieces) {
                    if (pos.second <= upSecNearest && pos.second > upNearest) {
                        upSecNearest = pos.second;
                    }
                    if (pos.second >= downSecNearest && pos.second < downNearest) {
                        downSecNearest = pos.second;
                    }
                }
                for (const auto& [pos, type] : pieces) {
                    if (pos.second == upSecNearest && (int)type * (int)this->type < 0) {
                        upEnemy = pos.second;
                    }
                    if (pos.second == downSecNearest && (int)type * (int)this->type < 0) {
                        downEnemy = pos.second;
                    }
                }

                if (jiangShuaiPos.second == upEnemy
                        || jiangShuaiPos.second == downEnemy)
                    return true;
            } else if (myself.second == jiangShuaiPos.second) {  // 横向
                const auto& pieces = find(-1, myself.second, -1);

                int rightNearest = 10, rightSecNearest = 10, rightEnemy = 10;
                int leftNearest = 0, leftSecNearest = 0, leftEnemy = 0;
                for (const auto& [pos, type] : pieces) {
                    if (pos.first <= rightNearest && pos.first > myself.first) {
                        rightNearest = pos.first;
                    }
                    if (pos.first >= leftNearest && pos.first < myself.first) {
                        leftNearest = pos.first;
                    }
                }
                for (const auto& [pos, type] : pieces) {
                    if (pos.first <= rightSecNearest && pos.first > rightNearest) {
                        rightSecNearest = pos.first;
                    }
                    if (pos.first >= leftSecNearest && pos.first < leftNearest) {
                        leftSecNearest = pos.first;
                    }
                }
                for (const auto& [pos, type] : pieces) {
                    if (pos.second == rightSecNearest && (int)type * (int)this->type < 0) {
                        rightEnemy = pos.second;
                    }
                    if (pos.second == leftSecNearest && (int)type * (int)this->type < 0) {
                        leftEnemy = pos.second;
                    }
                }

                if (jiangShuaiPos.first == leftEnemy
                        || jiangShuaiPos.first == rightEnemy)
                    return true;
            }
        }
    }

    return false;
}

const std::list<Pos> Pao::getPossibleMoves() const {
    std::list<Pos> pos{};
    for (int x = 1; x <= 9; x++) {
        for (int y = 1; y <= 10; y++) {
            if (this->isValidMove(x, y) && Board::getBoard()->find(x, y, side()).empty()) {
                pos.emplace_back(std::make_pair(x, y));
            }
        }
    }
    return pos;
}

////////////////////////兵

bool BingZu::isBasicMove(int x, int y) const {
    if (this->side() == Board::getBoard()->side()) {
        if (y < 6) {
            if(x == this->x && y - this->y == 1)
                return true;
        } else if (y == 6) {
            if (this->y == y && std::abs(this->x - x) == 1)
                return true;
            else if (y - this->y == 1 && this->x == x)
                return true;
        } else {
            if (this->y == y && std::abs(this->x - x) == 1)
                return true;
            else if (this->x == x && y - this->y == 1)
                return true;
        }
    } else {
        if (y > 5) {
            if(x == this->x && y - this->y == -1)
                return true;
        } else if (y == 5) {
            if (this->y == y && std::abs(this->x - x) == 1)
                return true;
            else if (y - this->y == 1 && this->x == x)
                return true;
        } else {
            if (this->y == y && std::abs(this->x - x) == 1)
                return true;
            else if (this->x == x && y - this->y == -1)
                return true;
        }
    }

    return false;
}

bool BingZu::isThreat(const std::list<std::pair<Pos, Piece::PieceType>>& pieces) const {
    if (Board::getBoard()->isChecked(side()))
        return false;

    auto find = [=](int x = -1, int y = -1, int side = -1) {
        std::list<std::pair<Pos, Piece::PieceType>> list;
        for (const auto& [pos, type] : pieces) {
            if (pos.first == ((x == -1) ? pos.first : x)) {
                if (pos.second == ((y == -1) ? pos.second : y)) {
                    if (getSide(type) == ((side == -1) ? getSide(type) : side)) {
                        list.emplace_back(std::make_pair(pos, type));
                    }
                }
            }
        }
        return list;
    };

    Pos jiangShuaiPos, myself;
    for (const auto& [pos, type] : pieces) {
        if (getSide(this->type) != getSide(type)              // 棋子不属于我方
                && removeSide(type) == PieceType::JIANG_SHUAI)      // 棋子是对方的将/帅
            jiangShuaiPos = pos;
    }
    for (const auto& [pos, type] : pieces) {
        if (type == this->type) {
            myself = pos;

            if (myself.second == jiangShuaiPos.second
                    && std::abs(myself.first - jiangShuaiPos.first) == 1)
                return true;
            else if (myself.first == jiangShuaiPos.first
                     && std::abs(myself.second - jiangShuaiPos.second) == 1)
                return true;
        }
    }
    return false;
}

const std::list<Pos> BingZu::getPossibleMoves() const {
    std::list<Pos> pos{};
    for (int x = 1; x <= 9; x++) {
        for (int y = 1; y <= 10; y++) {
            if (this->isValidMove(x, y) && Board::getBoard()->find(x, y, side()).empty()) {
                pos.emplace_back(std::make_pair(x, y));
            }
        }
    }
    return pos;
}

////////////////////////
