class ListNode
{
    public:
        int rowNum;
        int colNum;
        char placedChess;
        ListNode *next;
        ListNode(int rowNumArg, int colNumArg, char placedChessArg, ListNode *nextarg)
        {
            rowNum = rowNumArg;
            colNum = colNumArg;
            placedChess = placedChessArg;
            next = nextarg;
        }
};