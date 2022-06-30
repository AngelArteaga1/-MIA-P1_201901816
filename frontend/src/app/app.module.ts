import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';
import { FormsModule } from '@angular/forms';
import { MatTreeModule } from '@angular/material/tree';
import { MatButtonModule } from '@angular/material/button';
import { MatIconModule } from '@angular/material/icon';
import { MatDialogModule } from '@angular/material/dialog';

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
import { ProfileConfirmComponent } from './components/profile-confirm/profile-confirm.component';
import { WithoutAccessComponent } from './components/without-access/without-access.component';
import { Page404Component } from './components/page404/page404.component';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { FileComponent } from './components/file/file.component';
import { DirectoryComponent } from './components/directory/directory.component';

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
    ProfileComponent,
    ProfileConfirmComponent,
    WithoutAccessComponent,
    Page404Component,
    FileComponent,
    DirectoryComponent
  ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    FormsModule,
    HttpClientModule,
    FontAwesomeModule,
    BrowserAnimationsModule,
    MatTreeModule,
    MatButtonModule,
    MatIconModule,
    MatDialogModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
