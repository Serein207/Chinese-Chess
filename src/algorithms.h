#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "board.h"
#include <QDebug>
//工具类
class Algorithms {
public:
    //判断僵局(无子可走)
    static bool isStalemate(bool side) {
        auto result = Board::getBoard()->find(-1, -1, side);

        for (const auto& [pos, type] : result) {
            auto piece = Board::getBoard()->pieces.at(pos);
            // qDebug() << pos <<' '<< piece->getPossibleMoves();
            if (!piece->getPossibleMoves().empty())
                return false;
        }

        return true;
    }

    // Piece::noThreat和Algorithms::isCheck的推荐辅助函数
    // 调用enemy的isThreat，如果全部false，则side方不被将军
    static inline bool isCheck(const std::list<std::pair<Pos, Piece::PieceType>>& pieces, bool side) {
        for (const auto& [pos, type] : pieces) {
            if (getSide(type) != side) {     // 是对方的棋子
                auto enemy = Board::getBoard()->pieces.at(pos);
                if(enemy->isThreat(pieces)) {  // 对我方有威胁
                    return true;
                }
            }
        }
        return false;
    }

    // side方被将军
    static bool isCheck(bool side) {
        return isCheck(Board::getBoard()->find(), side);
    }
};

#endif // ALGORITHMS_H
