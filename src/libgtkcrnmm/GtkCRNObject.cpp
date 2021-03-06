/* Copyright 2010-2016 CoReNum, INSA-Lyon, ENS-Lyon
 * 
 * This file is part of libgtkcrnmm.
 * 
 * libgtkcrnmm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * libgtkcrnmm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with libgtkcrnmm.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * file: GtkCRNObject.cpp
 * \author Yann LEYDIER
 */

#include <GtkCRNObject.h>
#include <CRNString.h>
#include <CRNData/CRNMap.h>
#include <CRNData/CRNVector.h>
#include <GtkCRNProp3.h>
#include <GdkCRNPixbuf.h>
#include <CRNi18n.h>

using namespace GtkCRN;

/*! Creates a widget to display an object. The display will not be updated if the object changes.
 * 
 * \param[in]	obj	the object to display
 * \return	a pointer Gtk::Widget. The user must free the pointer or use Gtk::manage.
 */
Gtk::Widget* GtkCRN::create_widget_from_object(const crn::Object *obj)
{
	if (!obj)
	{
		return new Gtk::Label(_("null object"));
	}
	// Map
	const crn::Map *m = static_cast<const crn::Map*>(obj);
	if (m)
	{
		if (m->IsEmpty())
		{
			Glib::ustring txt("<i>(empty)</i>");
			Gtk::Label *lab = Gtk::manage(new Gtk::Label(txt));
			lab->set_use_markup();
			return lab;
		}
		else
		{
			Gtk::Expander *expander = new Gtk::Expander("Map");
#ifdef CRN_USING_GTKMM3
			Gtk::Grid *tab = Gtk::manage(new Gtk::Grid);
#else
			Gtk::Table *tab = Gtk::manage(new Gtk::Table(int(m->Size()), 2));
#endif
			//tab->set_col_spacings(4);
			expander->add(*tab);
			int row = 0;
			for (crn::Map::const_iterator it = m->begin(); it != m->end(); ++it)
			{
				//tab->attach(*Gtk::manage(new Gtk::Label(it->first.CStr())), 0, 1, row, row + 1, Gtk::FILL, Gtk::FILL);
				//tab->attach(*Gtk::manage(create_widget_from_object(it->second.get())), 1, 2, row, row + 1, Gtk::FILL|Gtk::EXPAND, Gtk::FILL);
				row += 1;
			}
			tab->show_all();
			return expander;
		}
	}
	// Vector
	const crn::Vector *v = static_cast<const crn::Vector*>(obj);
	if (v)
	{
		if (v->IsEmpty())
		{
			Glib::ustring txt("<i>(empty)</i>");
			Gtk::Label *lab = Gtk::manage(new Gtk::Label(txt));
			lab->set_use_markup();
			return lab;
		}
		else
		{
			Gtk::Expander *expander = new Gtk::Expander("Vector");
			Gtk::VBox *vbox = Gtk::manage(new Gtk::VBox);
			expander->add(*vbox);
			for (crn::Vector::const_iterator it = v->begin(); it != v->end(); ++it)
			{
				vbox->pack_start(*Gtk::manage(create_widget_from_object((*it).get())), false, true, 0);
			}
			vbox->show_all();
			return expander;
		}
	}
	// Prop3
	const crn::Prop3 *p = static_cast<const crn::Prop3*>(obj);
	if (p)
	{
		GtkCRN::Prop3 *pw = new GtkCRN::Prop3(Gtk::ICON_SIZE_BUTTON, *p);
		pw->set_sensitive(false);
		return pw;
	}
	// String
	const auto *s = static_cast<const crn::String*>(obj);
	if (s)
	{
		return new Gtk::Label(s->CStr());
	}
	// StringUTF8 and Path
	const auto *su = static_cast<const crn::StringUTF8*>(obj);
	if (su)
	{
		return new Gtk::Label(su->CStr());
	}
	// TODO XXX add Int and Real
	// Other
	return new Gtk::Label(typeid(*obj).name());
}

