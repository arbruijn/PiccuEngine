/*
* Piccu Engine
* Copyright (C) 2024 SaladBadger
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "gl_local.h"

static GLuint fbVAOName;
static GLuint fbVBOName;

static const float framebuffer_buffer[] = { -1.0f, -3.0f, 0.0f, 0.0f,
					  -1.0f, 1.0f, 0.0f, 2.0f,
					  3.0f, 1.0f, 2.0f, 2.0f };

void GL_InitFramebufferVAO(void)
{
	if (fbVAOName)
		return;

	glGenVertexArrays(1, &fbVAOName);
	glGenBuffers(1, &fbVBOName);

	glBindVertexArray(fbVAOName);
	glBindBuffer(GL_ARRAY_BUFFER, fbVBOName);
	glBufferData(GL_ARRAY_BUFFER, sizeof(framebuffer_buffer), framebuffer_buffer, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (GLvoid*)(sizeof(float) * 2));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void GL_DestroyFramebufferVAO(void)
{
	glDeleteBuffers(1, &fbVBOName);
	glDeleteVertexArrays(1, &fbVAOName);
	fbVBOName = fbVAOName = 0;
}

Framebuffer::Framebuffer()
{
	m_width = m_height = 0;
	m_name = m_colorname = m_depthname = 0;
}

void Framebuffer::Update(int width, int height)
{
	if (width == m_width && height == m_height)
		return;

	m_width = width;
	m_height = height;

	glActiveTextureARB(GL_TEXTURE0_ARB);
	if (m_name == 0)
	{
		glGenTextures(1, &m_colorname);
		glGenTextures(1, &m_depthname);
		glGenFramebuffers(1, &m_name);
	}

	GLenum textureType = GL_TEXTURE_2D; //future MSAA support?

	glBindTexture(GL_TEXTURE_2D, m_colorname);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, OpenGL_state.screen_width, OpenGL_state.screen_height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, m_depthname);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, OpenGL_state.screen_width, OpenGL_state.screen_height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	OpenGL_last_bound[0] = -1;
	OpenGL_last_bound[1] = -1;
	Last_texel_unit_set = -1;

	glBindFramebuffer(GL_FRAMEBUFFER, m_name);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureType, m_colorname, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textureType, m_depthname, 0);

	GLenum fbstatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fbstatus != GL_FRAMEBUFFER_COMPLETE)
	{
		Error("Framebuffer::Update: Framebuffer object is incomplete!");
	}
}

void Framebuffer::Destroy()
{
	glDeleteTextures(1, &m_colorname);
	glDeleteTextures(1, &m_depthname);
	glDeleteFramebuffers(1, &m_name);
	m_name = m_colorname = m_depthname = 0;
}

void Framebuffer::BlitToRaw(GLuint target, unsigned int x, unsigned int y, unsigned int w, unsigned int h)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_name);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, target);

	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		mprintf((0, "Error unbinding draw framebuffer: %d\n", err));
	}

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBlitFramebuffer(0, 0, m_width, m_height,
		x, y, x + w, y + h, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void Framebuffer::BlitTo(GLuint target, unsigned int x, unsigned int y, unsigned int w, unsigned int h)
{
	glBindVertexArray(fbVAOName);
	GLint oldviewport[4];
	glGetIntegerv(GL_VIEWPORT, oldviewport);
	glViewport(x, y, w, h);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, target);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	Last_texel_unit_set = 0;
	glClientActiveTextureARB(GL_TEXTURE0_ARB);
	glActiveTextureARB(GL_TEXTURE0_ARB);
	OpenGL_last_bound[0] = m_colorname;
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_colorname);
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		mprintf((0, "Error unbinding draw framebuffer: %d\n", err));
	}

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindVertexArray(0);
	glViewport(oldviewport[0], oldviewport[1], oldviewport[2], oldviewport[3]);
}