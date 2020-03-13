#include "cBasicTextureManager.h"

#include <sstream>
#include <thread>

static cBasicTextureManager* instance;
void cBasicTextureManager::SetBasePath(std::string basepath)
{
	this->m_basePath = basepath;
	return;
}


cBasicTextureManager* cBasicTextureManager::getInstance()
{
	if (!instance)
		instance = new cBasicTextureManager();
	return instance;
}

bool cBasicTextureManager::Create2DTextureFromBMPFile( std::string textureFileName, bool bGenerateMIPMap )
{
	std::string fileToLoadFullPath = this->m_basePath + "/" + textureFileName;


	CTextureFromBMP* pTempTexture = new CTextureFromBMP();
	if ( ! pTempTexture->CreateNewTextureFromBMPFile2( textureFileName, fileToLoadFullPath, /*textureUnit,*/ bGenerateMIPMap ) )
	{
		this->m_appendErrorString( "Can't load " );
		this->m_appendErrorString( fileToLoadFullPath );
		this->m_appendErrorString( "\n" );
		return false;
	}

	// Texture is loaded OK
	//this->m_nextTextureUnitOffset++;
	//

	pTempTexture->status = CTextureFromBMP::eTextureStatus::on_gpu;
	
	this->m_map_TexNameToTexture[ textureFileName ] = pTempTexture;

	return true;
}

bool cBasicTextureManager::Create2DTextureFromBMPFile_Threaded(std::string textureFileName, bool bGenerateMIPMap)
{
	auto data = new sTextureThreadData();

	data->texture = new CTextureFromBMP();
	data->texture->m_textureName = textureFileName;
	data->texture->m_fileNameFullPath = this->m_basePath + "\\" + textureFileName;
	data->texture_manager = this;

	this->m_map_TexNameToTexture[textureFileName] = data->texture;

	std::thread thread(load_texture_threaded_function, data);
	thread.detach();


	return true;
}

int cBasicTextureManager::push_loaded_textures_to_gpu()
{
	int count = 0;
	for (auto& x : m_map_TexNameToTexture)
	{
		if (x.second->status == CTextureFromBMP::eTextureStatus::ready_to_goto_gpu)
		{
			if (!x.second->push_texture_to_gpc())
			{
				x.second->status = CTextureFromBMP::eTextureStatus::error;
				continue;
			}
			count++;
		}
	}


	return count;
}


void cBasicTextureManager::m_appendErrorString( std::string nextErrorText )
{
	std::stringstream ss;
	ss << this->m_lastError << nextErrorText;
	this->m_lastError = ss.str();
	return;
}

GLuint cBasicTextureManager::getTextureIDFromName( std::string textureFileName )
{
	std::map< std::string, CTextureFromBMP* >::iterator itTexture
		= this->m_map_TexNameToTexture.find( textureFileName );
	// Found it?
	if ( itTexture == this->m_map_TexNameToTexture.end() || itTexture->second->status != CTextureFromBMP::eTextureStatus::on_gpu)
	{
		//return 0;
		return (GLuint)std::numeric_limits<unsigned>::max;
	}
	// Reutrn texture number (from OpenGL genTexture)
	return itTexture->second->getTextureNumber();
}


void cBasicTextureManager::m_appendErrorStringLine( std::string nextErrorTextLine )
{
	std::stringstream ss;
	ss << this->m_lastError << std::endl;
	ss << nextErrorTextLine << std::endl;
	this->m_lastError = ss.str();
	return;
}


// Picks a random texture from the textures loaded
std::string cBasicTextureManager::PickRandomTexture(void)
{
	if ( this->m_map_TexNameToTexture.empty() )
	{
		// There are no textures loaded, yet.
		return "";
	}

	int textureIndexRand = rand() % (this->m_map_TexNameToTexture.size() + 1);
	if ( textureIndexRand >= this->m_map_TexNameToTexture.size() )
	{
		textureIndexRand = 0;
	}

	std::map< std::string, CTextureFromBMP* >::iterator itTex = this->m_map_TexNameToTexture.begin();
	for ( unsigned int count = 0; count != textureIndexRand; count++, itTex++ ) 
	{}

	return itTex->second->getTextureName();
}


bool cBasicTextureManager::CreateCubeTextureFromBMPFiles( 
                                    std::string cubeMapName, 
		                            std::string posX_fileName, std::string negX_fileName, 
		                            std::string posY_fileName, std::string negY_fileName, 
									std::string posZ_fileName, std::string negZ_fileName, 
									bool bIsSeamless, std::string &errorString )
{
	std::string posX_fileName_FullPath = this->m_basePath + "/" + posX_fileName;
	std::string negX_fileName_FullPath = this->m_basePath + "/" + negX_fileName;
	std::string posY_fileName_FullPath = this->m_basePath + "/" + posY_fileName;
	std::string negY_fileName_FullPath = this->m_basePath + "/" + negY_fileName;
	std::string posZ_fileName_FullPath = this->m_basePath + "/" + posZ_fileName;
	std::string negZ_fileName_FullPath = this->m_basePath + "/" + negZ_fileName;


	GLenum errorEnum;
	std::string errorDetails;
	CTextureFromBMP* pTempTexture = new CTextureFromBMP();
	if ( ! pTempTexture->CreateNewCubeTextureFromBMPFiles( 
				cubeMapName, 
				posX_fileName_FullPath, negX_fileName_FullPath, 
	            posY_fileName_FullPath, negY_fileName_FullPath, 
	            posZ_fileName_FullPath, negZ_fileName_FullPath, 
	            bIsSeamless, errorEnum, errorString, errorDetails ) )
	{
		this->m_appendErrorString( "Can't load " );
		this->m_appendErrorString( cubeMapName );
		this->m_appendErrorString( " because:\n" );
		this->m_appendErrorString( errorString );
		this->m_appendErrorString( "\n" );
		this->m_appendErrorString( errorDetails );
		errorString += ("\n" + errorDetails);
		return false;
	}//if ( ! pTempTexture->CreateNewCubeTextureFromBMPFiles()

	// Texture is loaded OK
	//this->m_nextTextureUnitOffset++;
	
	this->m_map_TexNameToTexture[ cubeMapName ] = pTempTexture;

	return true;
}

