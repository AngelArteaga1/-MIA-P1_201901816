import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { TreeComponent } from './components/tree/tree.component';
import { LoginComponent } from './components/login/login.component';
import { RegisterComponent } from './components/register/register.component';
import { UsersComponent } from './components/users/users.component';
import { RegisterConfirmComponent } from './components/register-confirm/register-confirm.component';
import { RecoverPasswordComponent } from './components/recover-password/recover-password.component';
import { RecoverConfirmComponent } from './components/recover-confirm/recover-confirm.component';
import { ProfileComponent } from './components/profile/profile.component';
import { ProfileConfirmComponent } from './components/profile-confirm/profile-confirm.component';
import { WithoutAccessComponent } from './components/without-access/without-access.component';
import { Page404Component } from './components/page404/page404.component';
import { FileComponent } from './components/file/file.component';
import { DirectoryComponent } from './components/directory/directory.component';

const routes: Routes = [
  { path: '', component: TreeComponent },
  { path: 'login', component: LoginComponent },
  { path: 'register', component: RegisterComponent },
  { path: 'users', component: UsersComponent },
  { path: 'registerConfirm', component: RegisterConfirmComponent },
  { path: 'recoverPassword/:id', component: RecoverPasswordComponent },
  { path: 'recoverConfirm', component: RecoverConfirmComponent },
  { path: 'profile/:username', component: ProfileComponent },
  { path: 'profileConfirm', component: ProfileConfirmComponent },
  { path: 'withoutAccess', component: WithoutAccessComponent },
  { path: 'file/:path', component: FileComponent },
  { path: 'directory/:path', component: DirectoryComponent },
  { path: '**', pathMatch: 'full', component: Page404Component }
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
