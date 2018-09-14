// TestRand.cpp : 定义控制台应用程序的入口点。
#include "stdafx.h"
#include <stdlib.h>
#include "MemObjectPool.h"



enum ECardType
{
	ECT_ROKET = 0,			//火箭
	ECT_BOMB = 1,			//炸蛋
	ECT_SINGLE = 2,			//单张
	ECT_DOUBLE = 3,			//对子
	ECT_THREE = 4,			//三张
	ECT_THREE_ONE = 5,		//三张一
	ECT_THREE_TWO = 6,		//三张二
	ECT_SQUENCE_1	= 7,	//单顺子
	ECT_SQUENCE_2	= 8,	//二顺子
	ECT_SQUENCE_3	= 9,	//三顺子
	ECT_FLY_WING	= 10,	//飞机带翅膀
	ECT_FOUR_SINGLE = 11,	//四带两张单牌
	ECT_FOUR_DOUBLE = 12,	//四带两个对子

};

struct TCard
{
	TCard(char c = 0, int n = 0)
	{
		cCard = c ;
		nCount = n;
	}

	unsigned short cCard : 8;
	unsigned short nCount : 8;
};


// 输入
std::vector<TCard> g_playerA;
std::vector<TCard> g_playerB;

int GetCardCount(std::vector<TCard>& vtCards, char cCard)
{
	for(int i = 0; i < vtCards.size(); i++)
	{
		if(vtCards[i].cCard == cCard)
		{
			return vtCards[i].nCount;
		}
	}

	return 0;
}

struct TMove
{
	TMove()
	{
		nPattern = -1;
		nMinCard = -1;
		nCardNum = 0;
	}

	TMove(std::vector<TCard>& vtTemp, int nP, int nMin, int cardnum)
	{
		vtCards = vtTemp;
		nPattern = nP;
		nCardNum = cardnum;
		nMinCard = nMin;
	}

	bool CheckCards(std::vector<TCard>& cards)
	{
		if(cards.size() != vtCards.size())
		{
			return false;
		}

		for(int i = 0; i < cards.size(); i++)
		{
			if(cards[i].nCount != GetCardCount(vtCards, cards[i].cCard))
			{
				return false;
			}
		}

		return true;
	}
	int nPattern;
	int nMinCard;
	int nCardNum;
	std::vector<TCard> vtCards;
};

struct TNode
{
	MEM_OBJECT_POOL_SUPORT(TNode);
	TNode()
	{
		m_nPlayer = 0;
	}

	~TNode()
	{
		m_nPlayer = 0;
		for(int i = 0; i < m_pNextList.size(); i++)
		{
			delete m_pNextList[i];
		}

		m_pNextList.clear();
	}
	TMove m_Move;
	std::vector<TNode*> m_pNextList;
	int m_nPlayer;


};


TNode* g_pRootNode = new TNode;
// 映射
char CardToValue(char cCard)
{
	switch(cCard)
	{
		case '3':
			cCard = 0;
			break;
		case '4':
			cCard = 1;
			break;
		case '5':
			cCard = 2;
			break;
		case '6':
			cCard = 3;
			break;
		case '7':
			cCard = 4;
			break;
		case '8':
			cCard = 5;
			break;
		case '9':
			cCard = 6;
			break;
		case 'S':
			cCard = 7;
			break;
		case 'J':
			cCard = 8;
			break;
		case 'Q':
			cCard = 9;
			break;
		case 'K':
			cCard = 10;
			break;
		case 'A':
			cCard = 11;
			break;
		case '2':
			cCard = 12;
			break;
		case 'Y':
			cCard = 13;
			break;
		case 'Z':
			cCard = 14;
			break;
		default:
		{
			printf("error: 无效的牌: %c\n", cCard);
		}

	}
	return cCard;
}


void TransCardToValue(std::vector<TCard>& cards)
{
	for(int i = 0; i < cards.size(); i++)
	{
		cards[i].cCard = CardToValue(cards[i].cCard);
	}

	return ;
}

char ValueToCard(char cValue)
{
	char cCard = 0;
	switch(cValue)
	{
		case 0:
			cCard = '3';
			break;
		case 1:
			cCard = '4';
			break;
		case 2:
			cCard = '5';
			break;
		case 3:
			cCard = '6';
			break;
		case 4:
			cCard = '7';
			break;
		case 5:
			cCard = '8';
			break;
		case 6:
			cCard = '9';
			break;
		case 7:
			cCard = 'S';
			break;
		case 8:
			cCard = 'J';
			break;
		case 9:
			cCard = 'Q';
			break;
		case 10:
			cCard = 'K';
			break;
		case 11:
			cCard = 'A';
			break;
		case 12:
			cCard = '2';
			break;
		case 13:
			cCard = 'Y';
			break;
		case 14:
			cCard = 'Z';
			break;

	}

	return cCard;
}

void TransValueToCard(std::vector<TCard>& cards)
{
	for(int i = 0; i < cards.size(); i++)
	{
		cards[i].cCard = ValueToCard(cards[i].cCard);
	}

	return ;
}

// 获得顺子
bool getSeq(std::vector<TCard>& vtCards, int nLength, char minCard, int nNum, int nPattern, std::vector<TMove>& vtMoves)
{
	int nCardCount = 0;
	for(int i = 0; i < vtCards.size(); i++)
	{
		nCardCount += vtCards[i].nCount;
	}

	std::vector<TMove> vtTempMove;

	std::vector<TCard> seq;
	seq.reserve(nLength);
	if (vtCards.size() >= nLength)
	{
		for (int i = 0; i < vtCards.size(); i++)
		{
			TCard nCurCard = vtCards[i];
			if ((seq.size() == 0 && nCurCard.cCard > minCard && nCurCard.cCard < 12 && nCurCard.nCount >= nNum) ||
			        (seq.size() != 0 && seq[seq.size() - 1].cCard + 1 == nCurCard.cCard && nCurCard.cCard < 12 && nCurCard.nCount >= nNum))
			{
				seq.insert(seq.end(), TCard(nCurCard.cCard, nNum));
			}
			else
			{
				seq.clear();
				if(nCurCard.cCard > minCard && nCurCard.cCard < 12 && nCurCard.nCount >= nNum)
				{
					seq.insert(seq.end(), TCard(nCurCard.cCard, nNum));
				}
			}

			if (seq.size() == nLength)
			{
				if(nCardCount == nLength * nNum)
				{
					vtMoves.clear();
					vtMoves.push_back( TMove(seq, nPattern, seq[0].cCard, nLength));
					return true;
				}

				vtTempMove.push_back( TMove(seq, nPattern, seq[0].cCard, nLength));
				seq.erase(seq.begin());
			}
		}
	}

	if(vtTempMove.size() < 0)
	{
		return true;
	}

	for(int i = vtTempMove.size() - 1; i >= 0; i--)
	{
		vtMoves.push_back(vtTempMove[i]);
	}

	return false;
}

// 获得飞机带翅膀
void getPlane(std::vector<TCard>& vtCards, int nLength, char minCard, int nPattern, std::vector<TMove>& vtMoves)
{
	return ;
	std::vector<TCard> seq;
	if (vtCards.size() >= nLength)
	{
		for (int i = 0; i < vtCards.size(); i++)
		{
			TCard nCurCard = vtCards[i];
			if ((seq.size() == 0 && nCurCard.cCard > minCard &&  nCurCard.cCard < 12 && nCurCard.nCount >= 3) ||
			        (seq.size() != 0 && seq[seq.size() - 1].cCard + 1 ==  nCurCard.cCard &&  nCurCard.cCard < 12 && nCurCard.nCount >= 3))
			{
				seq.push_back(TCard(nCurCard.cCard, 3));
			}
			else
			{
				seq.clear();
			}
		}
		if (seq.size() == nLength)
		{
			//	std::set<char> cardBset = cardset;
//
			//	for(int i = 0; i < seq.size(); i++)
			//	{
			//		cardBset.erase(seq[i]);
			//	}

// 		for (case in list(combinations(cardBs, length)))
// 				{
// 					newSeq = seq + list(case)
// 					         ans.append( { 'c': newSeq, 'p': length + 26, 's': seq[0] })
// 					         seq = seq[3:]
//
// 				}
		}
	}

	return ;

}

// 枚举可能的下一步
void getNextMove(std::vector<TCard>& vtCards, int nPattern, char minCard, int nCardNum, std::vector<TMove>& vtMoves)
{
	int nCardCount = 0;
	for(int i = 0; i < vtCards.size(); i++)
	{
		nCardCount += vtCards[i].nCount;
	}

	// 不出
	if ((nPattern != -1))
	{
		std::vector<TCard>vtT;
		vtMoves.push_back(TMove(vtT, -1, -1, 0));
	}

// 单个牌
	if (nPattern == -1)
	{
		for (int i = vtCards.size() - 1; i >= 0; i--)
		{
			TCard& nCurCard = vtCards[i];
			if (nCurCard.cCard > minCard && nCurCard.nCount == 4)
			{
				std::vector<TCard>vtT;
				vtT.push_back(TCard(nCurCard.cCard, 1));
				vtMoves.push_back(TMove(vtT, ECT_SINGLE, nCurCard.cCard, 1));
			}
		}

		for (int i = vtCards.size() - 1; i >= 0; i--)
		{
			TCard& nCurCard = vtCards[i];
			if (nCurCard.cCard > minCard && nCurCard.nCount == 3)
			{
				std::vector<TCard>vtT;
				vtT.push_back(TCard(nCurCard.cCard, 1));
				vtMoves.push_back(TMove(vtT, ECT_SINGLE, nCurCard.cCard, 1));
			}
		}

		for (int i = vtCards.size() - 1; i >= 0; i--)
		{
			TCard& nCurCard = vtCards[i];
			if (nCurCard.cCard > minCard && nCurCard.nCount == 2)
			{
				std::vector<TCard>vtT;
				vtT.push_back(TCard(nCurCard.cCard, 1));
				vtMoves.push_back(TMove(vtT, ECT_SINGLE, nCurCard.cCard, 1));
			}
		}

		int nIndex = vtMoves.size();

		for (int i = vtCards.size() - 1; i >= 0; i--)
		{
			TCard& nCurCard = vtCards[i];
			if (nCurCard.cCard > minCard && nCurCard.nCount == 1)
			{
				std::vector<TCard>vtT;
				vtT.push_back(TCard(nCurCard.cCard, 1));
				vtMoves.push_back(TMove(vtT, ECT_SINGLE, nCurCard.cCard, 1));
			}
		}

		if(vtMoves.size() > nIndex + 1)
		{
			TMove tMove = vtMoves[vtMoves.size() - 2];
			vtMoves[vtMoves.size() - 2] = vtMoves[vtMoves.size() - 1];
			vtMoves[vtMoves.size() - 1] = tMove;
		}
	}

	if (nPattern == ECT_SINGLE)
	{
		for (int i = vtCards.size() - 1; i >= 0; i--)
		{
			TCard& nCurCard = vtCards[i];
			if (nCurCard.cCard > minCard  && nCurCard.nCount == 4)
			{
				std::vector<TCard>vtT;
				vtT.push_back(TCard(nCurCard.cCard, 1));
				vtMoves.push_back(TMove(vtT, ECT_SINGLE, nCurCard.cCard, 1));
			}
		}

		for (int i = vtCards.size() - 1; i >= 0; i--)
		{
			TCard& nCurCard = vtCards[i];
			if (nCurCard.cCard > minCard && nCurCard.nCount == 3)
			{
				std::vector<TCard>vtT;
				vtT.push_back(TCard(nCurCard.cCard, 1));
				vtMoves.push_back(TMove(vtT, ECT_SINGLE, nCurCard.cCard, 1));
			}
		}

		for (int i = vtCards.size() - 1; i >= 0; i--)
		{
			TCard& nCurCard = vtCards[i];
			if (nCurCard.cCard > minCard && nCurCard.nCount == 2)
			{
				std::vector<TCard>vtT;
				vtT.push_back(TCard(nCurCard.cCard, 1));
				vtMoves.push_back(TMove(vtT, ECT_SINGLE, nCurCard.cCard, 1));
			}
		}

		for (int i = vtCards.size() - 1; i >= 0; i--)
		{
			TCard& nCurCard = vtCards[i];
			if (nCurCard.cCard > minCard && nCurCard.nCount == 1)
			{
				std::vector<TCard>vtT;
				vtT.push_back(TCard(nCurCard.cCard, 1));
				vtMoves.push_back(TMove(vtT, ECT_SINGLE, nCurCard.cCard, 1));
			}
		}
	}

// 对子牌
	if ((nPattern == -1) || (nPattern == ECT_DOUBLE))
	{
		for (int i = vtCards.size() - 1; i >= 0; i--)
		{
			TCard& nCurCard = vtCards[i];

			if (nCurCard.nCount == 3 && nCurCard.cCard > minCard)
			{
				if(nCardCount == 2)
				{
					vtMoves.clear();
					std::vector<TCard>vtT;
					vtT.push_back(TCard(nCurCard.cCard, 2));
					vtMoves.push_back(TMove(vtT, ECT_DOUBLE, nCurCard.cCard, 2));
					return ;
				}

				std::vector<TCard>vtT;
				vtT.push_back(TCard(nCurCard.cCard, 2));
				vtMoves.push_back(TMove(vtT, ECT_DOUBLE, nCurCard.cCard, 2));
			}

			if (nCurCard.nCount == 2 && nCurCard.cCard > minCard)
			{
				if(nCardCount == 2)
				{
					vtMoves.clear();
					std::vector<TCard>vtT;
					vtT.push_back(TCard(nCurCard.cCard, 2));
					vtMoves.push_back(TMove(vtT, ECT_DOUBLE, nCurCard.cCard, 2));
					return ;
				}

				std::vector<TCard>vtT;
				vtT.push_back(TCard(nCurCard.cCard, 2));
				vtMoves.push_back(TMove(vtT, ECT_DOUBLE, nCurCard.cCard, 2));
			}
		}
	}

	// 火箭
	if ((GetCardCount(vtCards, 13) > 0) && (GetCardCount(vtCards, 14) > 0))
	{
		if(nCardCount == 2)
		{
			vtMoves.clear();
			std::vector<TCard>vtT;
			vtT.push_back(TCard(13, 1));
			vtT.push_back(TCard(14, 1));
			vtMoves.push_back(TMove(vtT, ECT_ROKET, 100, 2));
			return ;
		}

		std::vector<TCard>vtT;
		vtT.push_back(TCard(13, 1));
		vtT.push_back(TCard(14, 1));
		vtMoves.push_back(TMove(vtT, ECT_ROKET, 100, 2));
	}



// 三张牌
	if (nPattern == -1 || nPattern == ECT_THREE)
	{
		for (int i = 0; i < vtCards.size(); i++)
		{
			TCard& nCurCard = vtCards[i];
			if (nCurCard.nCount >= 3 && nCurCard.cCard > minCard)
			{
				if(nCardCount == 3)
				{
					vtMoves.clear();
					std::vector<TCard>vtT;
					vtT.push_back(TCard(nCurCard.cCard, 3));
					vtMoves.push_back(TMove(vtT, ECT_THREE, nCurCard.cCard, 0));
					return ;
				}

				std::vector<TCard>vtT;
				vtT.push_back(TCard(nCurCard.cCard, 3));
				vtMoves.push_back(TMove(vtT, ECT_THREE, nCurCard.cCard, 0));
			}
		}
	}

// 三带一
	if (nPattern == -1 || nPattern == ECT_THREE_ONE)
	{
		for (int i = vtCards.size() - 1; i >= 0; i--)
		{
			TCard& nCurCard = vtCards[i];
			if (nCurCard.nCount >= 3 && nCurCard.cCard > minCard)
			{
				for (int j = vtCards.size() - 1; j >= 0; j--)
				{
					if (nCurCard.cCard != vtCards[j].cCard)
					{
						if(nCardCount == 4)
						{
							vtMoves.clear();
							std::vector<TCard>vtT;
							vtT.push_back(TCard(nCurCard.cCard, 3));
							vtT.push_back(TCard(vtCards[j].cCard, 1));
							vtMoves.push_back(TMove(vtT, ECT_THREE_ONE, nCurCard.cCard, 0));
							return ;
						}

						std::vector<TCard>vtT;
						vtT.push_back(TCard(nCurCard.cCard, 3));
						vtT.push_back(TCard(vtCards[j].cCard, 1));
						vtMoves.push_back(TMove(vtT, ECT_THREE_ONE, nCurCard.cCard, 0));
					}
				}
			}
		}
	}

	// 炸弹
	if (nPattern != 0)
	{
		for (int i = 0; i < vtCards.size(); i++)
		{
			TCard& nCurCard = vtCards[i];
			if (nCurCard.nCount == 4 && (nPattern != 1 || (nPattern == 1 && nCurCard.cCard > minCard)))
			{
				if(nCardCount == 4)
				{
					vtMoves.clear();
					std::vector<TCard>vtT;
					vtT.push_back(TCard(nCurCard.cCard, 4));
					vtMoves.push_back(TMove(vtT, ECT_BOMB, nCurCard.cCard, 4));
					return ;
				}

				std::vector<TCard>vtT;
				vtT.push_back(TCard(nCurCard.cCard, 4));
				vtMoves.push_back(TMove(vtT, ECT_BOMB, nCurCard.cCard, 4));
			}
		}
	}
// 三带二
	if (nPattern == -1 || nPattern == ECT_THREE_TWO)
	{
		for (int i = vtCards.size() - 1; i >= 0; i--)
		{
			TCard& nCurCard = vtCards[i];
			if (nCurCard.nCount >= 3 && nCurCard.cCard > minCard)
			{
				for (int j = vtCards.size() - 1; j >= 0; j--)
				{
					if (nCurCard.cCard != vtCards[j].cCard && vtCards[j].nCount >= 2)
					{
						if(nCardCount == 4)
						{
							vtMoves.clear();
							std::vector<TCard>vtT;
							vtT.push_back(TCard(nCurCard.cCard, 3));
							vtT.push_back(TCard(vtCards[j].cCard, 2));
							vtMoves.push_back(TMove(vtT, ECT_THREE_TWO, nCurCard.cCard, 0));
							return ;
						}

						std::vector<TCard>vtT;
						vtT.push_back(TCard(nCurCard.cCard, 3));
						vtT.push_back(TCard(vtCards[j].cCard, 2));
						vtMoves.push_back(TMove(vtT, ECT_THREE_TWO, nCurCard.cCard, 0));
					}
				}
			}
		}
	}

// 单顺子
	if (nPattern == -1)
	{
		for (int length = 5; length <= 13; length++)
		{
			if(getSeq(vtCards, length, -1, 1, ECT_SQUENCE_1,  vtMoves))
			{
				return ;
			}
		}
	}

	if (nPattern == ECT_SQUENCE_1)
	{
		if(getSeq(vtCards, nCardNum, minCard, 1, ECT_SQUENCE_1, vtMoves))
		{
			return ;
		}
	}

// 双顺子
	if (nPattern == -1)
	{
		for (int length = 3; length <= 11; length++)
		{
			if(getSeq(vtCards, length, -1, 2, ECT_SQUENCE_2, vtMoves))
			{
				return ;
			}
		}
	}
	if (nPattern == ECT_SQUENCE_2)
	{
		if(getSeq(vtCards, nCardNum, minCard, 2, ECT_SQUENCE_2,  vtMoves))
		{
			return ;
		}
	}
// 三顺子
	if (nPattern == -1)
	{
		for (int length = 2; length <= 7; length++)
		{
			getSeq(vtCards, length, -1, 3, ECT_SQUENCE_3, vtMoves);
		}
	}
	if (nPattern == ECT_SQUENCE_3)
	{
		getSeq(vtCards, nCardNum, minCard, 3, ECT_SQUENCE_3,  vtMoves);
	}

// 飞机带翅膀
	if (nPattern == -1)
	{
		for (int length = 2; length <= 7; length++)
		{
			getPlane(vtCards, length, -1, ECT_FLY_WING, vtMoves);
		}
	}
	if (nPattern >= 28 && nPattern <= 32)
	{
		getPlane(vtCards, nCardNum, minCard, ECT_FLY_WING, vtMoves);
	}
// 四带两张单牌
	if (nPattern == -1 || nPattern == ECT_FOUR_SINGLE)
	{
		for (int i = 0; i < vtCards.size(); i++)
		{
			TCard& nCurCard = vtCards[i];
			if (nCurCard.nCount >= 4 && nCurCard.cCard > minCard)
			{
				//cardBs = list(cardset - set([card]))
				//     for case in list(combinations(cardBs, 2)):
				//{
				//moves.append({ 'c': [card] * 4 + list(case), 'p': 33, 's': card });
				//}
			}
		}
	}
// 四带两个对子
	if (nPattern == -1 || nPattern == ECT_FOUR_DOUBLE)
	{
		for (int i = 0; i < vtCards.size(); i++)
		{
			TCard nCurCard = vtCards[i];
			if ( nCurCard.nCount >= 4 && nCurCard.cCard > minCard)
			{
				std::vector<char> vtTwoCard;
				for (int j = vtCards.size() - 1; j >= 0; j--)
				{
					if (nCurCard.cCard != vtCards[j].cCard && vtCards[j].nCount >= 2)
					{
						vtTwoCard.push_back(vtCards[j].cCard);
					}
				}

				if(vtTwoCard.size() == 2)
				{
					std::vector<TCard>vtT;
					vtT.push_back(TCard(nCurCard.cCard, 4));
					vtT.push_back(TCard(vtTwoCard[0], 2));
					vtT.push_back(TCard(vtTwoCard[1], 2));
					vtMoves.push_back(TMove(vtT, ECT_FOUR_DOUBLE, nCurCard.cCard, 0));
				}
			}
		}
	}

	//if(vtMoves.size() > 1)
	//{
	//	vtMoves.erase(vtMoves.begin());
	//}

	return ;
}

// 从 list 中删掉另一个 list
void removeElements(std::vector<TCard>& vtCardA, std::vector<TCard>& vtCardB)
{
	for(int i = 0; i < vtCardB.size(); i++)
	{
		for(int j = 0; j < vtCardA.size(); j++)
		{
			if(vtCardA[j].cCard == vtCardB[i].cCard)
			{
				vtCardA[j].nCount -= vtCardB[i].nCount;
				if(vtCardA[j].nCount == 0)
				{
					vtCardA.erase(vtCardA.begin() + j);
				}
				break;
			}
		}
	}
}

// 对抗树
bool SearchTree(std::vector<TCard>& playerA, std::vector<TCard>& playerB, TMove& tMove, TNode* pParent, int nLevel)
{
	TNode* pNodeA =  new TNode;
	pNodeA->m_nPlayer = 1;
	std::vector<TMove> movesA;
	std::vector<TCard> newPlayerA;
	getNextMove(playerA, tMove.nPattern, tMove.nMinCard, tMove.nCardNum, movesA);
	for (int i = movesA.size() - 1; i >= 0; i--)
	{
		if(nLevel == 0)
		{
			printf("---%d-----\n", i);
		}

		newPlayerA = playerA;
		removeElements(newPlayerA, movesA[i].vtCards);

		pNodeA->m_Move = movesA[i];
		if (newPlayerA.size() == 0)
		{
			pParent->m_pNextList.push_back(pNodeA);
			return true;
		}

		bool Lose = false;
		std::vector<TMove> movesB;
		std::vector<TCard> newPlayerB;
		getNextMove(playerB, movesA[i].nPattern, movesA[i].nMinCard, movesA[i].nCardNum, movesB);
		for (int j = movesB.size() - 1; j >= 0; j--)
		{
			newPlayerB = playerB;
			removeElements(newPlayerB, movesB[j].vtCards);

			if (newPlayerB.size() == 0)
			{
				Lose = true;
				break;
			}

			TNode* pNodeB =  new TNode;
			pNodeB->m_Move = movesB[j];
			pNodeB->m_nPlayer = 2;
			pNodeA->m_pNextList.push_back(pNodeB);
			if(false == SearchTree(newPlayerA, newPlayerB, movesB[j], pNodeB, nLevel + 1))
			{
				Lose = true;
				break;
			}
		}

		if(!Lose)
		{
			pParent->m_pNextList.push_back(pNodeA);
			return true;
		}
		else
		{
			pNodeA->m_pNextList.clear();
		}
	}

	delete pNodeA;
	return false;
}


void StringToCards(std::vector<TCard>& g_playerA, char* pChar)
{
	while(*pChar != 0)
	{
		bool bFind = false;
		for(int i = 0; i < g_playerA.size(); i++)
		{
			if(g_playerA[i].cCard == *pChar)
			{
				g_playerA[i].nCount += 1;

				bFind = true;
			}
		}

		if(!bFind)
		{
			g_playerA.push_back(TCard(*pChar, 1));
		}

		pChar++;
	}
}


void TraceTree(TNode* pRoot)
{
	if(pRoot == NULL)
	{
		return ;
	}
	if(pRoot->m_nPlayer == 1)
	{
		printf("玩家:");
	}
	else
	{
		printf("电脑:");
	}
	if(pRoot->m_Move.vtCards.size() <= 0)
	{
		printf("不要");
	}
	else
	{
		for(int i = 0; i < pRoot->m_Move.vtCards.size(); i++)
		{
			printf("[%d张%C]", pRoot->m_Move.vtCards[i].nCount, ValueToCard(pRoot->m_Move.vtCards[i].cCard));
		}
	}

	printf("\n\n");

	if(pRoot->m_pNextList.size() > 0)
	{
		TraceTree(pRoot->m_pNextList[0]);
	}
}

int main()
{
	//72
	//3589SSJJQQKA2
	//57789SSJJKA2Y

	//98
	//35699SJQKA2
	//3446678SQKA2Y

	char szBuff[20] = {0};
	printf("请输入地主牌:");
	scanf("%s", szBuff);

	char* pChar = szBuff;
	StringToCards(g_playerA, pChar);
	TransCardToValue(g_playerA);

	printf("请输入农民牌:");
	memset(szBuff, 0, 20);
	scanf("%s", szBuff);
	pChar = szBuff;
	StringToCards(g_playerB, pChar);
	TransCardToValue(g_playerB);

	g_pRootNode->m_Move.nPattern = -1;
	g_pRootNode->m_Move.nMinCard = -1;
	g_pRootNode->m_nPlayer = 2;

	// 搜索对抗树
	int nTick = GetTickCount();
	TMove tMove;
	if(SearchTree(g_playerA, g_playerB, tMove, g_pRootNode, 0))
	{
		printf("--成功--时间:%d!!\n", GetTickCount() - nTick);
	}
	else
	{
		printf("--失败--时间:%d!!\n", GetTickCount() - nTick);
	}


	TraceTree(g_pRootNode);

	TNode* pCurNode = g_pRootNode->m_pNextList[0];
	while(pCurNode != NULL)
	{
		printf("地主出牌: ") ;
		if(pCurNode->m_Move.vtCards.size() <= 0)
		{
			printf("不要");
		}
		else
		{
			for(int i = 0; i < pCurNode->m_Move.vtCards.size(); i++)
			{
				printf("--[%c-%d]--", ValueToCard(pCurNode->m_Move.vtCards[i].cCard), pCurNode->m_Move.vtCards[i].nCount);
			}
		}

		printf("\n");

		if(pCurNode->m_pNextList.size() <= 0)
		{
			printf("己经胜利!!!\n");
			getchar();
			break;
		}

		printf("农民出牌:") ;

		char temp[20] = {0};
		scanf("%s", temp);

		std::vector<TCard> vtTemp;

		int nPatten = 0;
		if(strcmp(temp, "mm") == 0)
		{
			nPatten = -1;
		}
		else
		{
			StringToCards( vtTemp, temp);
			TransCardToValue(vtTemp);
		}

		bool bFind = false;
		for(int i = 0; i < pCurNode->m_pNextList.size(); i++)
		{
			if( nPatten == pCurNode->m_pNextList[i]->m_Move.nPattern && nPatten == -1)
			{
				pCurNode = pCurNode->m_pNextList[i]->m_pNextList[0];
				bFind =  true;
				break;
			}

			if(pCurNode->m_pNextList[i]->m_Move.CheckCards(vtTemp))
			{
				pCurNode = pCurNode->m_pNextList[i]->m_pNextList[0];
				bFind =  true;
				break;
			}
		}

		if(bFind == false)
		{
			printf("找不到解决方案!!");
			getchar();
		}
	}

	getchar();
	return 0;
}

