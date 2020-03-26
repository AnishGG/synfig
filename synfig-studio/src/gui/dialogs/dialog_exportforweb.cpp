/* === S Y N F I G ========================================================= */
/*!	\file dialog_exportforweb.cpp
**	\brief The class Dialog_ExportForWeb is defined here
**
**	$Id$
**
**	\legal
**	Copyright (c) 2002-2005 Robert B. Quattlebaum Jr., Adrian Bentley
**
**	This package is free software; you can redistribute it and/or
**	modify it under the terms of the GNU General Public License as
**	published by the Free Software Foundation; either version 2 of
**	the License, or (at your option) any later version.
**
**	This package is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
**	General Public License for more details.
**	\endlegal
*/
/* ========================================================================= */

/* === H E A D E R S ======================================================= */

#ifdef USING_PCH
#	include "pch.h"
#else
#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include <synfig/general.h>

#include "dialog_exportforweb.h"
#include <gtkmm/builder.h>
#include "canvasview.h"
#include "workarea.h"

#include <gui/localization.h>

#include "app.h"
#include "instance.h"
#include "gui/resourcehelper.h"
#include "widgets/widget_vector.h"

#endif

/* === U S I N G =========================================================== */

using namespace std;
using namespace etl;
using namespace synfig;
using namespace studio;

/* === M A C R O S ========================================================= */

/* === G L O B A L S ======================================================= */

/* === P R O C E D U R E S ================================================= */

/* === M E T H O D S ======================================================= */

static Glib::RefPtr<Gtk::Builder> load_interface() {
	auto refBuilder = Gtk::Builder::create();
	try
	{
		refBuilder->add_from_file(ResourceHelper::get_ui_path("dialog_exportforweb.glade"));     // will have to create dialog_exportforweb.glade
	}
	catch(const Glib::FileError& ex)
	{
		synfig::error("FileError: " + ex.what());
		return Glib::RefPtr<Gtk::Builder>();
	}
	catch(const Glib::MarkupError& ex)
	{
		synfig::error("MarkupError: " + ex.what());
		return Glib::RefPtr<Gtk::Builder>();
	}
	catch(const Gtk::BuilderError& ex)
	{
		synfig::error("BuilderError: " + ex.what());
		return Glib::RefPtr<Gtk::Builder>();
	}
	return refBuilder;
}

void Dialog_ExportForWeb::set_canvas_view(CanvasView* canvas_view)
{
	this->canvas_view_ = canvas_view;
	update_title();
}

Dialog_ExportForWeb::Dialog_ExportForWeb(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) :
	Gtk::Dialog(cobject),
	builder(refGlade)//,
//	canvas_view_(canvas_view),
{
	refGlade->get_widget("outline_without_variable_width", outline_without_variable_width);

  /* Find the plugin id from here and also the path*/
    std::list<PluginManager::plugin> plugin_list = App::plugin_manager.get_list();
    for(std::list<PluginManager::plugin>::const_iterator p = plugin_list.begin(); p != plugin_list.end(); ++p){
        if(p->path.find("lottie-exporter") != std::string::npos){
            plugin_path = p->path;
        }
    }

	Gtk::Button *button = nullptr;

	refGlade->get_widget("ok_button", button);
	if (button)
		button->signal_clicked().connect(sigc::mem_fun(*this, &studio::Dialog_ExportForWeb::on_ok_pressed));

	refGlade->get_widget("apply_button", button);
	if (button)
		button->signal_clicked().connect(sigc::mem_fun(*this, &studio::Dialog_ExportForWeb::on_apply_pressed));

	refGlade->get_widget("cancel_button", button);
	if (button)
		button->signal_clicked().connect(sigc::mem_fun(*this, &studio::Dialog_ExportForWeb::on_cancel_pressed));

    if (outline_without_variable_width)
        outline_without_variable_width->signal_toggled().connect(sigc::mem_fun(*this, &studio::Dialog_ExportForWeb::on_outline_without_variable_width_toggle));

	signal_show().connect(sigc::mem_fun(*this, &studio::Dialog_ExportForWeb::refresh));

	update_title();
}

Dialog_ExportForWeb* Dialog_ExportForWeb::create(Gtk::Window& parent, CanvasView* canvas_view){
	auto refBuilder = load_interface();
	if (!refBuilder)
		return nullptr;
	Dialog_ExportForWeb* dialog = nullptr;
	refBuilder->get_widget_derived("dialog_exportforweb", dialog);
	if (dialog) {
		dialog->set_transient_for(parent);
		dialog->set_canvas_view(canvas_view);
	}
	return dialog;
}

Dialog_ExportForWeb::~Dialog_ExportForWeb()
{
}

void
Dialog_ExportForWeb::update_title()
{
	if (canvas_view_)
		set_title(_("Export for web")+String(" - ")+canvas_view_->get_canvas()->get_name());
}

void
Dialog_ExportForWeb::refresh()
{
    /* outline_without_variable_width->set_active(true); */
	update_title();
}


void
Dialog_ExportForWeb::on_outline_without_variable_width_toggle()
{
}

void
Dialog_ExportForWeb::on_apply_pressed()
{

	/*canvas_view_->set_grid_show_toggle(outline_without_variable_width->get_active());*/

    canvas_view_->get_instance()->run_plugin(this->plugin_path);
}

void
Dialog_ExportForWeb::on_ok_pressed()
{
	on_apply_pressed();
	hide();
}

void
Dialog_ExportForWeb::on_cancel_pressed()
{
	refresh();
	hide();
}
