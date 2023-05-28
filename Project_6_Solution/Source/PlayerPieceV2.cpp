#include "PlayerPieceV2.h"

#include "Application.h"
#include "Collider.h"
#include "Path.h"
#include "ModulePuzzlePiecesV3.h"

PlayerPieceV2::PlayerPieceV2()
{
	pieces[0][0] = nullptr;
	pieces[0][1] = nullptr;
	pieces[1][0] = nullptr;
	pieces[1][1] = nullptr;

	piecePaths[0][0] = new Path();
	piecePaths[0][1] = new Path();
	piecePaths[1][0] = new Path();
	piecePaths[1][1] = new Path();

	piecePaths[0][0]->name = "TopLeft";
	piecePaths[0][1]->name = "TopRight";
	piecePaths[1][0]->name = "BotLeft";
	piecePaths[1][1]->name = "BotRight";



	// Impide que se mueva si el jugador no le da a la tecla de rotacion
	piecePaths[0][0]->PushBack({ 0,0 }, 1);
	piecePaths[0][1]->PushBack({ 0,0 }, 1);
	piecePaths[1][0]->PushBack({ 0,0 }, 1);
	piecePaths[1][1]->PushBack({ 0,0 }, 1);

	piecePaths[0][0]->PushBack({ 4,0 }, 3);
	piecePaths[0][1]->PushBack({ 0,4 }, 3);
	piecePaths[1][0]->PushBack({ 0,-4 }, 3);
	piecePaths[1][1]->PushBack({ -4,0 }, 3);

	piecePaths[0][0]->loop = false;
	piecePaths[0][1]->loop = false;
	piecePaths[1][0]->loop = false;
	piecePaths[1][1]->loop = false;

	piecePaths[0][0]->finished = true;
	piecePaths[0][1]->finished = true;
	piecePaths[1][0]->finished = true;
	piecePaths[1][1]->finished = true;
}

PlayerPieceV2::PlayerPieceV2(PuzzlePiece* pieces[4])
{
	setPieces(pieces);
}

PlayerPieceV2::~PlayerPieceV2()
{
	CleanUp();
}

bool PlayerPieceV2::Start()
{
	return true;
}

bool PlayerPieceV2::CleanUp()
{
	for (size_t i = 0; i < 2; i++)
	{
		for (size_t j = 0; j < 2; j++)
		{
			if (pieces[i][j] != nullptr)
				pieces[i][j] = nullptr; // La limpieza de piezas la hace el modulo principal
		}
	}

	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			if (colliders[i][j] != nullptr) {
				colliders[i][j]->pendingToDelete = true;
				colliders[i][j] = nullptr;
			}
		}
	}

	return true;
}

bool PlayerPieceV2::Rotate()
{
	//Si rotar hace que se solape con otra pieza no continues
	if (rotating || App->pieces->WillCollide(PlayerCollisionCheck::CENTER)) {
		return false;
	}

	//Rota las piezas
	rotating = true;

	for (size_t i = 0; i < 2; i++)
	{
		for (size_t j = 0; j < 2; j++)
		{
			piecePaths[i][j]->Reset();
		}
	}


	return true;
}

bool PlayerPieceV2::Update()
{
	for (size_t i = 0; i < 2; i++)
	{
		for (size_t j = 0; j < 2; j++)
		{
			if (pieces[i][j] == nullptr) continue;
			pieces[i][j]->position.x = position.x + PIECE_SIZE * j;
			pieces[i][j]->position.y = position.y + PIECE_SIZE * i;
			pieces[i][j]->position += piecePaths[i][j]->GetRelativePosition();
			pieces[i][j]->Update();
		}
	}

	if (rotating) {
		// Gira en el sentido del reloj por defecto (hara falta reimplementar para poder cambiar la direccion) (TODO)
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				piecePaths[i][j]->Update();
			}
		}


		if (piecePaths[0][0]->finished) {
			rotating = false;
			PuzzlePiece* aux = pieces[0][0];
			pieces[0][0] = pieces[1][0];
			pieces[1][0] = pieces[1][1];
			pieces[1][1] = pieces[0][1];
			pieces[0][1] = aux;

			for (size_t i = 0; i < 2; i++)
			{
				for (size_t j = 0; j < 2; j++)
				{
					piecePaths[i][j]->Reset();
				}
			}

		}
	}

	return true;
}

void PlayerPieceV2::refreshColliderState()
{
	//Compara con el array de piezas para activar o desactivar colisiones
	for (size_t i = 0; i < 2; i++)
	{
		for (size_t j = 0; j < 2; j++)
		{
			if (colliders[1+i][1+j] != nullptr) {

			}
		}
	}
}

void PlayerPieceV2::setPieces(PuzzlePiece* newPieces[4])
{
	for (uint i = 0; i < 4; i++)
	{
		newPieces[i]->moving = true;
		// Check if type==NONE
	}
	pieces[0][0] = newPieces[0];
	pieces[0][1] = newPieces[1];
	pieces[1][0] = newPieces[2];
	pieces[1][1] = newPieces[3];

	pieces[0][0]->name = "TopLeft";
	pieces[0][1]->name = "TopRight";
	pieces[1][0]->name = "BotLeft";
	pieces[1][1]->name = "BotRight";

	App->pieces->playArea.watched = pieces[1][0]; // TODO eliminar variable debug
}
