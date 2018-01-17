#include "logic.h"
#include <QList>
#include <QByteArray>
#include <QHash>
#include <QFileDialog>
#include <QApplication>
#include <QMessageBox>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTime>

struct Figure
{
    int type;
    QPoint f_Coords;
};


struct Logic::Impl
{
    QList<Figure> figures;

    int findByPosition(int x, int y);
    bool checkColorMatch(const int index_1, const int index_2);
    bool getFigureColor(const int index);

    // For bishop, rook coords calculate;
    void getVerticalHorizontalDiagonal_Coords(QVector<QPoint>& emptyCoords, QVector<QPoint>& versusCoords, int& coordsX, int& coordsY, int& startStepX, int& startStepY);
};

int Logic::Impl::findByPosition(int x, int y)
{
    for (int i(0); i < figures.size(); ++i)
    {
        if (figures.at(i).f_Coords.x() != x || figures.at(i).f_Coords.y() != y )
        {
            continue;
        }
        return i;
    }
    return -1;
}

bool Logic::Impl::checkColorMatch(const int index_1, const int index_2)
{
    int index_1Type = figures.at(index_1).type;
    int index_2Type = figures.at(index_2).type;
    bool bCheck_1, bCheck_2, bResult;

    QVector<int> whiteFiguresVec = { w_Rook, w_Knight, w_Bishop, w_Queen, w_King, w_Pawn };

    whiteFiguresVec.contains(index_1Type) ? bCheck_1 = true : bCheck_1 = false;
    whiteFiguresVec.contains(index_2Type) ? bCheck_2 = false : bCheck_2 = true;
    bCheck_1 != bCheck_2 ? bResult = false : bResult = true;

    return bResult;
}

bool Logic::Impl::getFigureColor(const int index)
{
    QVector<int> whiteFiguresVec = { w_Rook, w_Knight, w_Bishop, w_Queen, w_King, w_Pawn };

    if(whiteFiguresVec.contains(figures.at(index).type))
        return true;
    else
        return false;
}

void Logic::Impl::getVerticalHorizontalDiagonal_Coords(QVector<QPoint> &emptyCoords, QVector<QPoint> &versusCoords, int &coordsX, int &coordsY, int &startStepX, int &startStepY)
{
    bool bCheck = true;
    bool sign_x, sign_y;

    startStepX > 0 ? sign_x = true : sign_x = false;
    startStepY > 0 ? sign_y = true : sign_y = false;

    while((coordsX + startStepX <= 8 && coordsX + startStepX >= 0) && (coordsY + startStepY <= 8 && coordsY + startStepY >= 0) && bCheck)
    {
        if(findByPosition(coordsX + startStepX, coordsY + startStepY) == -1)
        {
            emptyCoords.append(QPoint(coordsX + startStepX, coordsY + startStepY));
        }

        if(findByPosition(coordsX + startStepX, coordsY + startStepY) >= 0)
        {
            versusCoords.append(QPoint(coordsX + startStepX, coordsY + startStepY));
            bCheck = false;
        }

        if(startStepX != 0)
            sign_x ? ++startStepX : --startStepX;
        if(startStepY != 0)
            sign_y ? ++startStepY : --startStepY;
    }
}


Logic::Logic(QObject *parent) : QAbstractListModel(parent), impl(new Impl())
{
    this->initGame();
}

Logic::~Logic()
{

}

int Logic::rowCount(const QModelIndex & ) const
{
    return impl->figures.size();
}

QHash<int, QByteArray> Logic::roleNames() const
{
    QHash<int, QByteArray> names;
    names.insert(Roles::Type        , "type");
    names.insert(Roles::PositionX   , "positionX");
    names.insert(Roles::PositionY   , "positionY");

    return names;
}

bool Logic::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent)
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    while(count--)
        impl->figures.removeAt(row);
    endRemoveRows();
    return true;
}

bool Logic::movePawn(int fromX, int fromY, int toX, int toY)
{
    int index = impl->findByPosition(fromX, fromY);
    int toIndex = impl->findByPosition(toX, toY);

    int fromStep = 0;
    int nextStep = 0;

    impl->getFigureColor(index) ? (fromStep = 6, nextStep = -1) : (fromStep = 1, nextStep = 1);

    // If Empty cell;
    if(impl->figures[index].f_Coords.y() == fromStep)
    {
        if( fromX == toX  && (toY == (fromY + nextStep) || toY == (fromY + (nextStep + nextStep))) )
        {
            if(impl->findByPosition(toX, toY) == -1)
            {
                impl->figures[index].f_Coords.setX(toX);
                impl->figures[index].f_Coords.setY(toY);
                return true;
            }
        }
    }
    else
    {
        if( fromX == toX  && (toY == (fromY + nextStep)) )
        {
            if(impl->findByPosition(toX, toY) == -1)
            {
                impl->figures[index].f_Coords.setX(toX);
                impl->figures[index].f_Coords.setY(toY);
                return true;
            }
        }
    }

    // If enemy detected;
    impl->getFigureColor(index) ? nextStep = -1 : nextStep = 1;
    if(toIndex != -1)
    {
        if( ((fromY + nextStep) == toY)  && (toX == (fromX - 1) || toX == (fromX + 1)) )
        {
            impl->figures[index].f_Coords.setX(toX);
            impl->figures[index].f_Coords.setY(toY);
            removeRows(toIndex, 1);
            return true;
        }
    }
    return false;
}

bool Logic::moveKnight(int fromX, int fromY, int toX, int toY)
{
    int index = impl->findByPosition(fromX, fromY);
    int toIndex = impl->findByPosition(toX, toY);

    // Check for right step;
    if(((fromX - 1) == toX && (fromY - 2) == toY) ||
            ((fromX + 1) == toX && (fromY - 2) == toY) ||
            ((fromX + 2) == toX && (fromY - 1) == toY) ||
            ((fromX + 2) == toX && (fromY + 1) == toY) ||
            ((fromX + 1) == toX && (fromY + 2) == toY) ||
            ((fromX - 1) == toX && (fromY + 2) == toY) ||
            ((fromX - 2) == toX && (fromY + 1) == toY) ||
            ((fromX - 2) == toX && (fromY - 1) == toY))
    {
        // If cell is empty;
        if(toIndex == -1)
        {
            impl->figures[index].f_Coords.setX(toX);
            impl->figures[index].f_Coords.setY(toY);
            return true;
        }
        // If cell is not empty;
        else
        {
            if(impl->checkColorMatch(index,toIndex))
            {
                {
                    impl->figures[index].f_Coords.setX(toX);
                    impl->figures[index].f_Coords.setY(toY);
                    removeRows(toIndex, 1);
                    return true;
                }
            }
        }
    }
    return false;
}

bool Logic::moveBishop(int fromX, int fromY, int toX, int toY)
{
    int index = impl->findByPosition(fromX, fromY);
    int toIndex = impl->findByPosition(toX, toY);

    // Get possible steps;
    QVector<QPoint> emptyCellsAround;
    QVector<QPoint> versusCellsAround;
    int cntX = 0;
    int cntY = 0;
    for(int i(0); i < 4; ++i)
    {
        if(i == 0)
        {
            cntX = 1;
            cntY = -1;
            impl->getVerticalHorizontalDiagonal_Coords(emptyCellsAround, versusCellsAround, fromX, fromY, cntX, cntY);
        }
        if(i == 1)
        {
            cntX = 1;
            cntY = 1;
            impl->getVerticalHorizontalDiagonal_Coords(emptyCellsAround, versusCellsAround, fromX, fromY, cntX, cntY);
        }
        if(i == 2)
        {
            cntX = -1;
            cntY = +1;
            impl->getVerticalHorizontalDiagonal_Coords(emptyCellsAround, versusCellsAround, fromX, fromY, cntX, cntY);
        }
        if(i == 3)
        {
            cntX = -1;
            cntY = -1;
            impl->getVerticalHorizontalDiagonal_Coords(emptyCellsAround, versusCellsAround, fromX, fromY, cntX, cntY);
        }
    }
    return analizeVecsData(emptyCellsAround, versusCellsAround, index, toIndex, toX, toY);
}

bool Logic::moveRook(int fromX, int fromY, int toX, int toY)
{
    int index = impl->findByPosition(fromX, fromY);
    int toIndex = impl->findByPosition(toX, toY);

    // Get possible steps;
    QVector<QPoint> emptyCellsAround;
    QVector<QPoint> versusCellsAround;
    int cntX = 0;
    int cntY = 0;
    for(int i(0); i < 4; ++i)
    {
        if(i == 0)
        {
            cntX = 0;
            cntY = -1;
            impl->getVerticalHorizontalDiagonal_Coords(emptyCellsAround, versusCellsAround, fromX, fromY, cntX, cntY);
        }
        if(i == 1)
        {
            cntX = 1;
            cntY = 0;
            impl->getVerticalHorizontalDiagonal_Coords(emptyCellsAround, versusCellsAround, fromX, fromY, cntX, cntY);
        }
        if(i == 2)
        {
            cntY = 1;
            cntX = 0;
            impl->getVerticalHorizontalDiagonal_Coords(emptyCellsAround, versusCellsAround, fromX, fromY, cntX, cntY);
        }
        if(i == 3)
        {
            cntX = -1;
            cntY = 0;
            impl->getVerticalHorizontalDiagonal_Coords(emptyCellsAround, versusCellsAround, fromX, fromY, cntX, cntY);
        }
    }
    return analizeVecsData(emptyCellsAround, versusCellsAround, index, toIndex, toX, toY);
}

bool Logic::moveQueen(int fromX, int fromY, int toX, int toY)
{
    int index = impl->findByPosition(fromX, fromY);
    int toIndex = impl->findByPosition(toX, toY);

    // Get possible steps;
    QVector<QPoint> emptyCellsAround;
    QVector<QPoint> versusCellsAround;
    int cntX = 0;
    int cntY = 0;
    for(int i(0); i < 8; ++i)
    {
        // Rook steps;
        if(i == 0)
        {
            cntX = 0;
            cntY = -1;
            impl->getVerticalHorizontalDiagonal_Coords(emptyCellsAround, versusCellsAround, fromX, fromY, cntX, cntY);
        }
        if(i == 1)
        {
            cntX = 1;
            cntY = 0;
            impl->getVerticalHorizontalDiagonal_Coords(emptyCellsAround, versusCellsAround, fromX, fromY, cntX, cntY);
        }
        if(i == 2)
        {
            cntY = 1;
            cntX = 0;
            impl->getVerticalHorizontalDiagonal_Coords(emptyCellsAround, versusCellsAround, fromX, fromY, cntX, cntY);
        }
        if(i == 3)
        {
            cntX = -1;
            cntY = 0;
            impl->getVerticalHorizontalDiagonal_Coords(emptyCellsAround, versusCellsAround, fromX, fromY, cntX, cntY);
        }
        // Bishop steps;
        if(i == 4)
        {
            cntX = 1;
            cntY = -1;
            impl->getVerticalHorizontalDiagonal_Coords(emptyCellsAround, versusCellsAround, fromX, fromY, cntX, cntY);
        }
        if(i == 5)
        {
            cntX = 1;
            cntY = 1;
            impl->getVerticalHorizontalDiagonal_Coords(emptyCellsAround, versusCellsAround, fromX, fromY, cntX, cntY);
        }
        if(i == 6)
        {
            cntX = -1;
            cntY = +1;
            impl->getVerticalHorizontalDiagonal_Coords(emptyCellsAround, versusCellsAround, fromX, fromY, cntX, cntY);
        }
        if(i == 7)
        {
            cntX = -1;
            cntY = -1;
            impl->getVerticalHorizontalDiagonal_Coords(emptyCellsAround, versusCellsAround, fromX, fromY, cntX, cntY);
        }
    }
    return analizeVecsData(emptyCellsAround, versusCellsAround, index, toIndex, toX, toY);
}

bool Logic::moveKing(int fromX, int fromY, int toX, int toY)
{
    int index = impl->findByPosition(fromX, fromY);
    int toIndex = impl->findByPosition(toX, toY);

    QVector<QPoint> emptyCellsAround;
    QVector<QPoint> versusCellsAround;

    // If empty cells;
    int startX = impl->figures[index].f_Coords.x() - 1;
    int startY = impl->figures[index].f_Coords.y() - 1;
    for(int i(0); i < 3; ++i)
    {
        for(int j(0); j < 3; ++j)
        {
            if((startX + i >= 0 && startX + i <= 8) && (startY + j >= 0 && startY + j < 8))
            {
                if(impl->findByPosition(startX + i, startY + j) == -1)
                    emptyCellsAround.append(QPoint(startX + i, startY + j));
                if(impl->findByPosition(startX + i, startY + j) >= 0)
                {
                    versusCellsAround.append(QPoint(startX + i, startY + j));
                }
            }
        }
    }
    return analizeVecsData(emptyCellsAround, versusCellsAround, index, toIndex, toX, toY);
}

bool Logic::analizeVecsData(const QVector<QPoint> emptyCells, const QVector<QPoint> versusCells, const int _index, const int _toIndex, int to_X, int to_Y)
{
    QPoint toPoint(to_X, to_Y);
    if(emptyCells.contains(toPoint))
    {
        impl->figures[_index].f_Coords.setX(to_X);
        impl->figures[_index].f_Coords.setY(to_Y);
        return true;
    }
    if(versusCells.contains(toPoint))
    {
        if(impl->checkColorMatch(_index, _toIndex))
        {
            impl->figures[_index].f_Coords.setX(to_X);
            impl->figures[_index].f_Coords.setY(to_Y);
            removeRows(_toIndex, 1);
            return true;
        }
    }
    return false;
}

void Logic::saveHistory()
{
    m_vecHistory.append(impl->figures);
}

void Logic::clearHistory()
{
    m_vecHistory.clear();
}

void Logic::refreshModel(const int& index)
{
    QModelIndex topLeft = createIndex(index, 0);
    QModelIndex bottomRight = createIndex(index, 0);
    emit dataChanged(topLeft, bottomRight);
}

QVariant Logic::data(const QModelIndex & modelIndex, int role) const { 
    if (!modelIndex.isValid()) {
        return QVariant();
    }
    
    int index = static_cast<int>(modelIndex.row());

    if (index >= impl->figures.size() || index < 0) {
        return QVariant();
    }

    Figure &figure = impl->figures[index];
    
    switch (role) {
    case Roles::Type        : return figure.type;
    case Roles::PositionX   : return figure.f_Coords.x();
    case Roles::PositionY   : return figure.f_Coords.y();

    }
    return QVariant();
}

void Logic::clear() {
    beginResetModel();
    impl->figures.clear();
    endResetModel();
    initGame();
}

bool Logic::move(int fromX, int fromY, int toX, int toY)
{
    int index = impl->findByPosition(fromX, fromY);

    if (index < 0) return false;
    if (toX < 0 || toX >= BOARD_SIZE || toY < 0 || toY >= BOARD_SIZE)
    {
        return false;
    }

    bool retResult = false;

    // White moves;
    if(m_bWhiteBlackMove)
    {
        if(impl->figures[index].type == w_Pawn)
        {
            retResult = movePawn(fromX, fromY, toX, toY);
        }
        if(impl->figures[index].type == w_Knight)
        {
            retResult = moveKnight(fromX, fromY, toX, toY);
        }
        if(impl->figures[index].type == w_Bishop)
        {
            retResult = moveBishop(fromX, fromY, toX, toY);
        }
        if(impl->figures[index].type == w_Rook)
        {
            retResult = moveRook(fromX, fromY, toX, toY);
        }
        if(impl->figures[index].type == w_Queen)
        {
            retResult = moveQueen(fromX, fromY, toX, toY);
        }
        if(impl->figures[index].type == w_King)
        {
            retResult = moveKing(fromX, fromY, toX, toY);
        }
        if(retResult)
        {
            m_bWhiteBlackMove = false;
            if(cursorToVecPos != -1)
            {
                m_vecHistory.resize(cursorToVecPos);
            }
            saveHistory();
        }
        return retResult;
    }

    // Black moves;
    if(!m_bWhiteBlackMove)
    {
        if(impl->figures[index].type == b_Pawn)
        {
            retResult = movePawn(fromX, fromY, toX, toY);
        }
        if(impl->figures[index].type == b_Knight)
        {
            retResult = moveKnight(fromX, fromY, toX, toY);
        }
        if(impl->figures[index].type == b_Bishop)
        {
            retResult = moveBishop(fromX, fromY, toX, toY);
        }
        if(impl->figures[index].type == b_Rook)
        {
            retResult = moveRook(fromX, fromY, toX, toY);
        }
        if(impl->figures[index].type == b_Queen)
        {
            retResult = moveQueen(fromX, fromY, toX, toY);
        }
        if(impl->figures[index].type == b_King)
        {
            retResult = moveKing(fromX, fromY, toX, toY);
        }
        if(retResult)
        {
            m_bWhiteBlackMove = true;
            saveHistory();
        }
        return retResult;
    }
    return false;
}

void Logic::resetGame()
{
    impl->figures.clear();
    initGame();
}

bool Logic::loadFile()
{
    QString filePath = QFileDialog::getOpenFileName(0, tr("Open File"), QApplication::applicationDirPath() + "/", tr("Save file (*.chs)"));

    if(filePath.isEmpty())
        return false;

    else
    {
        QFile file(filePath);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QString data = file.readAll();
        file.close();

        if(data.contains("ChessGameLevel-v.1.0"))
        {
            QJsonDocument doc(QJsonDocument::fromJson(data.toUtf8()));
            QJsonObject obj = doc.object();

            int arrSize = obj["HistorySize"].toInt();

            QList<Figure> lstFigures;
            Figure tmpFigure;

            m_vecHistory.clear();

            for(int i(0); i < arrSize; ++i)
            {
                QJsonArray array = obj["FiguresCoordTypes_" + QString::number(i)].toArray();
                for(int j(0); j < array.size(); )
                {
                    tmpFigure.f_Coords.setX(array.at(j).toInt());
                    tmpFigure.f_Coords.setY(array.at(j + 1).toInt());
                    tmpFigure.type = array.at(j + 2).toInt();
                    lstFigures.append(tmpFigure);
                    j+=3;
                }
                m_vecHistory.append(lstFigures);
                lstFigures.clear();
            }
            m_bWhiteBlackMove = obj["WhoseTurn"].toBool();

            impl->figures.clear();

            impl->figures = m_vecHistory.at(m_vecHistory.size() - 1);
            if(m_vecHistory.size() > 1)
                cursorToVecPos = m_vecHistory.size() - 1;
            beginResetModel();
            endResetModel();
        }
        else
        {
            QMessageBox::warning(0, "Error", "Invalid file, choose correct file");
        }
        return true;
    }
}

bool Logic::saveFile()
{
    QFile file;
    file.setFileName(m_pathToFile);

    if(!file.open(QIODevice::WriteOnly))
        return false;

    //create file with cell content
    QJsonObject jsonObject;
    for(int i(0); i < m_vecHistory.size(); ++i)
    {
        QJsonArray dataArr;
        for(int j(0); j < m_vecHistory.at(i).size(); ++j)
        {
            dataArr.append(m_vecHistory.at(i).at(j).f_Coords.x());
            dataArr.append(m_vecHistory.at(i).at(j).f_Coords.y());
            dataArr.append(m_vecHistory.at(i).at(j).type);
        }
        jsonObject["FiguresCoordTypes_" + QString::number(i)] = dataArr;
    }
    jsonObject["File-check"] = "ChessGameLevel-v.1.0";
    jsonObject["HistorySize"] = m_vecHistory.size();
    jsonObject["WhoseTurn"] = m_bWhiteBlackMove;

    QJsonDocument jsonDoc(jsonObject);
    file.write(jsonDoc.toJson());
    file.close();
    return true;
}

void Logic::nextStep()
{
    if(cursorToVecPos < m_vecHistory.size() - 1)
    {
        cursorToVecPos++;
        impl->figures.clear();
        impl->figures = m_vecHistory.at(cursorToVecPos);
        beginResetModel();
        endResetModel();
    }
}

void Logic::previousStep()
{
    if(cursorToVecPos > 0)
    {
        cursorToVecPos--;
        impl->figures.clear();
        impl->figures = m_vecHistory.at(cursorToVecPos);
        beginResetModel();
        endResetModel();
    }
}

void Logic::initGame()
{
    // Figures starting place initialization;
    // White figures;
    impl->figures << Figure { b_Rook,   QPoint(0, 0) };
    impl->figures << Figure { b_Knight, QPoint(1, 0) };
    impl->figures << Figure { b_Bishop, QPoint(2, 0) };
    impl->figures << Figure { b_Queen,  QPoint(3, 0) };
    impl->figures << Figure { b_King,   QPoint(4, 0) };
    impl->figures << Figure { b_Bishop, QPoint(5, 0) };
    impl->figures << Figure { b_Knight, QPoint(6, 0) };
    impl->figures << Figure { b_Rook,   QPoint(7, 0) };

    impl->figures << Figure { b_Pawn,   QPoint(0, 1) };
    impl->figures << Figure { b_Pawn,   QPoint(1, 1) };
    impl->figures << Figure { b_Pawn,   QPoint(2, 1) };
    impl->figures << Figure { b_Pawn,   QPoint(3, 1) };
    impl->figures << Figure { b_Pawn,   QPoint(4, 1) };
    impl->figures << Figure { b_Pawn,   QPoint(5, 1) };
    impl->figures << Figure { b_Pawn,   QPoint(6, 1) };
    impl->figures << Figure { b_Pawn,   QPoint(7, 1) };

    // Black figures;
    impl->figures << Figure { w_Rook,   QPoint(0, 7) };
    impl->figures << Figure { w_Knight, QPoint(1, 7) };
    impl->figures << Figure { w_Bishop, QPoint(2, 7) };
    impl->figures << Figure { w_Queen,  QPoint(3, 7) };
    impl->figures << Figure { w_King,   QPoint(4, 7) };
    impl->figures << Figure { w_Bishop, QPoint(5, 7) };
    impl->figures << Figure { w_Knight, QPoint(6, 7) };
    impl->figures << Figure { w_Rook,   QPoint(7, 7) };

    impl->figures << Figure { w_Pawn,   QPoint(0, 6) };
    impl->figures << Figure { w_Pawn,   QPoint(1, 6) };
    impl->figures << Figure { w_Pawn,   QPoint(2, 6) };
    impl->figures << Figure { w_Pawn,   QPoint(3, 6) };
    impl->figures << Figure { w_Pawn,   QPoint(4, 6) };
    impl->figures << Figure { w_Pawn,   QPoint(5, 6) };
    impl->figures << Figure { w_Pawn,   QPoint(6, 6) };
    impl->figures << Figure { w_Pawn,   QPoint(7, 6) };

    // White is start moving = true, black = false;
    m_bWhiteBlackMove = true;

    // Default path to save file;
    QTime currentTime = QTime::currentTime();
    QString sTime = currentTime.toString();
    sTime.remove(":");
    m_pathToFile = QApplication::applicationDirPath() + "/" + sTime + ".chs";
    cursorToVecPos = -1;
    saveHistory();
}






