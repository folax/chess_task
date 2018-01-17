#pragma once
#include <memory>
#include <QAbstractListModel>

// TIP: King = король, Queen = королева, Rook = ладья, Bishop = слон, Knight = конь, Pawn = пешка;
struct Figure;

class Logic: public QAbstractListModel
{
    Q_OBJECT
public:

    enum FigureName {
        b_King,     //0
        b_Queen,    //1
        b_Rook,     //2
        b_Bishop,   //3
        b_Knight,   //4
        b_Pawn,     //5
        w_King,     //6
        w_Queen,    //7
        w_Rook,     //8
        w_Bishop,   //9
        w_Knight,   //10
        w_Pawn,     //11
    };

    enum GlobalConstants {
        BOARD_SIZE = 8
    };

    enum Roles {
        Type = Qt::UserRole,
        PositionX,
        PositionY,
    };
    
public:
    explicit Logic(QObject *parent = 0);
    ~Logic();

    Q_PROPERTY(int boardSize READ boardSize CONSTANT);
    inline int boardSize() const { return BOARD_SIZE; }

    Q_INVOKABLE void clear();
    Q_INVOKABLE bool move(int fromX, int fromY, int toX, int toY);
    Q_INVOKABLE void resetGame();

    // Files operations;
    Q_INVOKABLE bool loadFile();
    Q_INVOKABLE bool saveFile();
    Q_INVOKABLE void nextStep();
    Q_INVOKABLE void previousStep();

    void initGame();
    void refreshModel(const int& index);

    // Figures moving;
    bool movePawn(int fromX, int fromY, int toX, int toY);
    bool moveKnight(int fromX, int fromY, int toX, int toY);
    bool moveBishop(int fromX, int fromY, int toX, int toY);
    bool moveRook(int fromX, int fromY, int toX, int toY);
    bool moveQueen(int fromX, int fromY, int toX, int toY);
    bool moveKing(int fromX, int fromY, int toX, int toY);
    bool analizeVecsData(const QVector<QPoint> emptyCells, const QVector<QPoint> versusCells, const int _index, const int _toIndex, int to_X, int to_Y);

    // History;
    void saveHistory();
    void clearHistory();

protected:
    int rowCount(const QModelIndex & parent) const override;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

private:
    struct Impl;
    std::unique_ptr<Impl> impl;

    bool m_bWhiteBlackMove;
    QVector<QList<Figure>> m_vecHistory;
    QString m_pathToFile;
    int cursorToVecPos;
};

