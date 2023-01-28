#include "board.h"
#include "algorithms.h"
#include "network.h"

using Type = Piece::PieceType;

Board::Board() {
    //Add your own code below
    //////////////////////////  finished
    connect(Network::getInstance(), &Network::move, this, &Board::onMove);
    //////////////////////////
}

void Board::judgeStatus() {
    checked[!side()] = Algorithms::isCheck(!side());
    checked[side()] = Algorithms::isCheck(side());
    if (Algorithms::isStalemate(side()))
        return emit win(!side());
    if (Algorithms::isStalemate(!side()))
        return emit win(side());
}

void Board::onSetup(Cell** cells) {
    for (int i = 0; i < 10 * 9; i++) {
        auto& cell = cells[i];
        this->cells.emplace(std::piecewise_construct, std::tuple(cell->x, cell->y), std::tuple(cell));
    }
    std::map<Type, Constructor> factory = {
        //请将nullptr替换为'new ClassName(x, y, side)'，请正确设置派生类构造函数参数    finished
        { Type::JIANG_SHUAI, [](int x, int y, bool side)->const Piece* { return new JiangShuai(x, y, side); } },
        { Type::SHI, [](int x, int y, bool side)->const Piece* { return new Shi(x, y, side); } },
        { Type::XIANG, [](int x, int y, bool side)->const Piece* { return new Xiang(x, y, side); } },
        { Type::MA, [](int x, int y, bool side)->const Piece* { return new Ma(x, y, side); } },
        { Type::JU, [](int x, int y, bool side)->const Piece* { return new Ju(x, y, side); } },
        { Type::PAO, [](int x, int y, bool side)->const Piece* { return new Pao(x, y, side); } },
        { Type::BING_ZU, [](int x, int y, bool side)->const Piece* { return new BingZu(x, y, side); } }
    };
    setPieces(factory);
    for (const auto& [pos, piece] : pieces)
        this->cells[pos]->change(piece.get());
    if (side())
        your_turn = true;

    //Add your own code below
    //////////////////////////


    //////////////////////////
}

void Board::setPieces(const std::map<Type, Constructor> & factory) {
    static std::list<std::pair<Pos, Type>> piece_list = {
        { { 1, 1 }, Type::RED_JU },
        { { 2, 1 }, Type::RED_MA },
        { { 3, 1 }, Type::RED_XIANG },
        { { 4, 1 }, Type::RED_SHI },
        { { 5, 1 }, Type::RED_SHUAI },
        { { 6, 1 }, Type::RED_SHI },
        { { 7, 1 }, Type::RED_XIANG },
        { { 8, 1 }, Type::RED_MA },
        { { 9, 1 }, Type::RED_JU },
        { { 2, 3 }, Type::RED_PAO },
        { { 8, 3 }, Type::RED_PAO },
        { { 1, 4 }, Type::RED_BING },
        { { 3, 4 }, Type::RED_BING },
        { { 5, 4 }, Type::RED_BING },
        { { 7, 4 }, Type::RED_BING },
        { { 9, 4 }, Type::RED_BING },
        { { 1, 10 }, Type::BLACK_JU },
        { { 2, 10 }, Type::BLACK_MA },
        { { 3, 10 }, Type::BLACK_XIANG },
        { { 4, 10 }, Type::BLACK_SHI },
        { { 5, 10 }, Type::BLACK_JIANG },
        { { 6, 10 }, Type::BLACK_SHI },
        { { 7, 10 }, Type::BLACK_XIANG },
        { { 8, 10 }, Type::BLACK_MA },
        { { 9, 10 }, Type::BLACK_JU },
        { { 2, 8 }, Type::BLACK_PAO },
        { { 8, 8 }, Type::BLACK_PAO },
        { { 1, 7 }, Type::BLACK_ZU },
        { { 3, 7 }, Type::BLACK_ZU },
        { { 5, 7 }, Type::BLACK_ZU },
        { { 7, 7 }, Type::BLACK_ZU },
        { { 9, 7 }, Type::BLACK_ZU }
    };

    for (const auto& [pos, type] : piece_list){
        const std::shared_ptr<const Piece> p(factory.at(removeSide(type))(pos.first, pos.second, side() == getSide(type)));
        pieces.emplace(pos, p);
    }
}

void Board::move(const Pos from, const Pos to) {
    const auto piece = pieces.at(from);
    piece->move(to.first, to.second);
    auto cell_from = cells.at(from);
    cell_from->change(nullptr);
    auto cell_to = cells.at(to);
    cell_to->change(piece.get());
    if (pieces.count(to) && removeSide(pieces.at(to)->type) == Type::JIANG_SHUAI)
        emit win(your_turn ? side() : !side());
    pieces[to] = piece;
    pieces.erase(from);
    judgeStatus();
}

void Board::onClick() {
    std::lock_guard guard(lock);
    int x = qobject_cast<Cell*>(sender())->x;
    int y = qobject_cast<Cell*>(sender())->y;
    static Piece const* selected = nullptr;
    if (!isYourTurn() || isMoved())
        return;
    const auto pos = std::make_pair(x, y);
    if (pieces.count(pos)) {
        const auto& piece = pieces.at(pos);
        if (piece->side() == side()) {
            selected = piece.get();
            cells.at(pos)->select();
            return;
        }
    }
    if (!selected)
        return;
    if (selected->isValidMove(x, y)) {
        cells.at(pos)->fineMove();
        Network::getInstance()->onMove(selected->pos(), pos);
        move(selected->pos(), pos);
        moved = true;
        your_turn = false;        
        selected = nullptr;   
    } else
        cells.at(pos)->wrongMove();
}

void Board::onMove(const Pos from, const Pos to) {
    std::lock_guard guard(lock);
    move(from, to);
    your_turn = true;
    moved = false;
}

const std::list<std::pair<Pos, Piece::PieceType>> Board::find(int x, int y, int side) const {
    //Add your own code here
    //////////////////////////  finished
    std::list<std::pair<Pos, Piece::PieceType>> list;
    for (auto& [pos, type] : this->pieces) {
        if (pos.first == ((x == -1) ? pos.first : x)) {
            if (pos.second == ((y == -1) ? pos.second : y)) {
                if (type->side() == ((side == -1) ? type->side() : side)) {
                    list.emplace_back(std::make_pair(pos, pieces.at(pos)->type));
                }
            }
        }
    }
    return list;
    //////////////////////////
}
