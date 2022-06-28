import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';
import { FormsModule } from '@angular/forms';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { LoginComponent } from './components/login/login.component';
import { TreeComponent } from './components/tree/tree.component';
import { HttpClientModule } from '@angular/common/http';
import { RegisterComponent } from './components/register/register.component';
import { UsersComponent } from './components/users/users.component';
import { RegisterConfirmComponent } from './components/register-confirm/register-confirm.component';
import { RecoverPasswordComponent } from './components/recover-password/recover-password.component';
import { RecoverConfirmComponent } from './components/recover-confirm/recover-confirm.component';
import { FontAwesomeModule } from '@fortawesome/angular-fontawesome';
import { ProfileComponent } from './components/profile/profile.component';

@NgModule({
  declarations: [
    AppComponent,
    LoginComponent,
    TreeComponent,
    RegisterComponent,
    UsersComponent,
    RegisterConfirmComponent,
    RecoverPasswordComponent,
    RecoverConfirmComponent,
    ProfileComponent
  ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    FormsModule,
    HttpClientModule,
    FontAwesomeModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
